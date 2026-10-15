"""Compile string templates and documented calls; opt into regression with --full."""
from pathlib import Path
import argparse
import os
import shutil
import subprocess
import tempfile
from compile_examples import ROOT, compile_examples

NAMES = ('trie', 'hash_single', 'hash_overflow', 'hash', 'kmp', 'exkmp', 'manacher', 'ac_automaton')


def check(temp, full=False):
    compile_examples(temp, 'strings', [f'code/strings/{name}.cpp' for name in NAMES],
                     [f'chapters/strings/{name}.tex' for name in NAMES])
    if not full:
        print('Compile only; behavioral regression skipped. Use --full when needed.', flush=True)
        return
    from check_hash import check as check_hash
    check_hash(temp)
    compiler = shutil.which('g++')
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
