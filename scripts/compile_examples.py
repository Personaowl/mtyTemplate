"""Compile and link documented examples in both integer modes without running them."""
from pathlib import Path
import os
import re
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[1]


def compile_examples(temp, name, sources, chapters, preamble=()):
    compiler = shutil.which('g++')
    if not compiler:
        raise RuntimeError('g++ is required in PATH')
    source = ['#include<bits/stdc++.h>', '#ifdef MTY_INT64',
              '#define int long long', '#endif', 'using namespace std;']
    source += list(preamble)
    source += [f'#include "{path}"' for path in sources]
    calls = []
    for path in chapters:
        tex = (ROOT / path).read_text(encoding='utf-8')
        examples = list(re.finditer(r'\\begin\{lstlisting\}(?:\[[^\]]*\])?\s*\n(.*?)\\end\{lstlisting\}', tex, flags=re.S))
        if r'\CodeFile{' in tex and not examples:
            raise RuntimeError(f'Source entry has no documented call: {path}')
        for match in examples:
            line = tex.count('\n', 0, match.start(1)) + 1
            calls.append('{\n' + f'#line {line} "{path}"\n' + match.group(1) + '\n}')
    if not calls:
        raise RuntimeError(f'No documented calls found: {name}')
    source.extend(['signed main(){', *calls, 'return 0;', '}'])
    cpp = temp / f'{name}_usage.cpp'
    cpp.write_text('\n'.join(source), encoding='utf-8')
    for widened in (False, True):
        mode = 'int64' if widened else 'int32'
        exe = temp / (f'{name}_{mode}' + ('.exe' if os.name == 'nt' else ''))
        args = [compiler, '-std=c++17', '-O0', '-Wall', '-Wextra',
                '-Wno-unused-variable', '-Wno-unused-but-set-variable',
                '-I', str(ROOT), str(cpp), '-o', str(exe)]
        if widened:
            args.insert(1, '-DMTY_INT64')
        subprocess.run(args, check=True, timeout=90)
        print(f'PASS: {name}: compiled and linked {len(sources)} templates, '
              f'{len(calls)} documented examples ({mode})', flush=True)
