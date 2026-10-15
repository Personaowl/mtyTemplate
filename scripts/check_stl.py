"""Compile C++17 STL examples; --smoke checks the maintained 128-bit I/O helper."""
from pathlib import Path
import argparse
import os
import shutil
import subprocess
import tempfile
from compile_examples import ROOT, compile_examples

NAMES = ('iterators', 'vector', 'string', 'pair_tuple', 'operators', 'queue', 'heap', 'set',
         'map', 'unordered', 'sort', 'binary_search', 'discretize', 'numeric',
         'permutation', 'bitset', 'builtins', 'int128')


def check(temp, smoke=False):
    compile_examples(temp, 'stl', ['code/stl/int128.cpp'],
                     [f'chapters/stl/{name}.tex' for name in NAMES])
    if not smoke:
        return
    compiler = shutil.which('g++')
    for widened in (False, True):
        exe = temp / ('stl64' if widened else 'stl32')
        if os.name == 'nt':
            exe = exe.with_suffix('.exe')
        args = [compiler, '-std=c++17', '-O2', '-Wall', '-Wextra', '-I', str(ROOT),
                str(ROOT / 'tests/stl.cpp'), '-o', str(exe)]
        if widened:
            args.insert(1, '-DMTY_INT64')
        subprocess.run(args, check=True, timeout=90)
        subprocess.run([str(exe)], check=True, timeout=60)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--smoke', action='store_true', help='also check 128-bit I/O boundaries')
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='mty-stl-') as directory:
        check(Path(directory), smoke=args.smoke)
