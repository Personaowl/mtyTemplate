"""Compile string templates and documented calls; opt into regression with --full."""
from pathlib import Path
import argparse
import os
import re
import shutil
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
NAMES = ('trie', 'hash', 'kmp', 'exkmp', 'manacher', 'ac_automaton')


def usage_source():
    source = ['#include<bits/stdc++.h>', '#ifdef MTY_INT64',
              '#define int long long', '#endif', 'using namespace std;']
    calls = []
    for name in NAMES:
        source.append(f'#include "code/strings/{name}.cpp"')
        tex = (ROOT / f'chapters/strings/{name}.tex').read_text(encoding='utf-8')
        tutorial = re.split(r'\\sub(?:sub)?section\*\{使用教程\}', tex, maxsplit=1)[1].split(r'\needspace', 1)[0]
        examples = re.findall(r'\\begin\{lstlisting\}(?:\[[^\]]*\])?\s*\n(.*?)\\end\{lstlisting\}',
                              tutorial, flags=re.S)
        if not examples:
            raise RuntimeError(f'No usage example found: {name}')
        for example in examples:
            calls.append('{\n' + example + '\n}')
    source.extend(['signed main(){', *calls, 'return 0;', '}'])
    return '\n'.join(source)


def check(temp, full=False):
    compiler = shutil.which('g++')
    if not compiler:
        raise RuntimeError('g++ is required in PATH')
    example_source = temp / 'strings_usage.cpp'
    example_source.write_text(usage_source(), encoding='utf-8')
    for widened in (False, True):
        mode = 'int64' if widened else 'int32'
        exe = temp / ('strings_usage_' + mode + ('.exe' if os.name == 'nt' else ''))
        # Example results may intentionally be unused; compile and link without running.
        args = [compiler, '-std=c++17', '-O0', '-Wall', '-Wextra',
                '-Wno-unused-variable', '-Wno-unused-but-set-variable',
                '-I', str(ROOT), str(example_source), '-o', str(exe)]
        if widened:
            args.insert(1, '-DMTY_INT64')
        subprocess.run(args, check=True, timeout=90)
        print(f'PASS: compiled and linked 6 string templates and their documented calls ({mode})', flush=True)
    if not full:
        print('Compile only; behavioral regression skipped. Use --full when needed.', flush=True)
        return
    for widened in (False, True):
        exe = temp / ('strings64' if widened else 'strings32')
        if os.name == 'nt':
            exe = exe.with_suffix('.exe')
        args = [compiler, '-std=c++17', '-O2', '-Wall', '-Wextra', '-I', str(ROOT),
                str(ROOT / 'tests/strings.cpp'), '-o', str(exe)]
        if widened:
            args.insert(1, '-DMTY_INT64')
        subprocess.run(args, check=True, timeout=90)
        subprocess.run([str(exe)], check=True, timeout=60)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--full', action='store_true', help='also run the existing behavioral regression')
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='mty-strings-') as directory:
        check(Path(directory), full=args.full)
