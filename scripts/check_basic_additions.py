"""Check binary Trie, 0-1 BFS, Euler walks, vertex blocks and Mobius with independent models."""
from pathlib import Path
import os
import shutil
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]


def check(temp):
    for widened in (False, True):
        exe = temp / ('basic_additions_' + str(widened) + ('.exe' if os.name == 'nt' else ''))
        args = [shutil.which('g++'), '-std=c++17', '-O2', '-Wall', '-Wextra',
                '-I', str(ROOT), str(ROOT / 'tests/basic_additions.cpp'), '-o', str(exe)]
        if widened:
            args.insert(1, '-DMTY_INT64')
        subprocess.run(args, check=True, timeout=90)
        subprocess.run([str(exe)], check=True, timeout=60)


if __name__ == '__main__':
    with tempfile.TemporaryDirectory(prefix='mty-basic-additions-') as directory:
        check(Path(directory))
