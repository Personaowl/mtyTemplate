"""Targeted checks for single-modulus and natural-overflow string hashes."""
from pathlib import Path
import os
import shutil
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]


def check(temp):
    compiler = shutil.which('g++')
    if not compiler:
        raise RuntimeError('g++ is required in PATH')
    for widened in (False, True):
        exe = temp / ('hash64' if widened else 'hash32')
        if os.name == 'nt':
            exe = exe.with_suffix('.exe')
        args = [compiler, '-std=c++17', '-O2', '-Wall', '-Wextra', '-I', str(ROOT),
                str(ROOT / 'tests/hash.cpp'), '-o', str(exe)]
        if widened:
            args.insert(1, '-DMTY_INT64')
        subprocess.run(args, check=True, timeout=90)
        subprocess.run([str(exe)], check=True, timeout=60)


if __name__ == '__main__':
    with tempfile.TemporaryDirectory(prefix='mty-hash-') as directory:
        check(Path(directory))
