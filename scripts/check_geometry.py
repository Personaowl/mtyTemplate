"""Compile geometry by default; --smoke runs exact/high-precision independent models."""
from pathlib import Path
import argparse
import os
import shutil
import subprocess
import tempfile
from compile_examples import ROOT, compile_examples
from geometry_oracle import check as check_oracle

NAMES = ('integer_point', 'point', 'segment', 'line', 'polygon', 'polygon_real', 'convex_hull', 'convex_query', 'calipers', 'closest_pair', 'minkowski', 'circle', 'tangents', 'triangle', 'enclosing_circle', 'circle_polygon', 'polygon_cut', 'halfplane', 'point3', 'space', 'volume', 'sphere')
SOURCES = [f'code/geometry/{name}.cpp' for name in NAMES]

def check_area(temp):
    """Only the changed shoelace implementation; no unrelated geometry regressions."""
    for widened in (False, True):
        exe = temp / ('geometry-area64' if widened else 'geometry-area32')
        if os.name == 'nt':
            exe = exe.with_suffix('.exe')
        args = [shutil.which('g++'), '-std=c++17', '-O2', '-Wall', '-Wextra', '-I', str(ROOT),
                str(ROOT / 'tests/geometry_area.cpp'), '-o', str(exe)]
        if widened:
            args.insert(1, '-DMTY_INT64')
        subprocess.run(args, check=True, timeout=180)
        subprocess.run([str(exe)], check=True, timeout=30)

def check(temp, smoke=False):
    compile_examples(temp, 'geometry', SOURCES, [f'chapters/geometry/{name}.tex' for name in NAMES])
    if not smoke:
        return
    check_area(temp)
    for widened in (False, True):
        exe = temp / ('geometry64' if widened else 'geometry32')
        if os.name == 'nt':
            exe = exe.with_suffix('.exe')
        args = [shutil.which('g++'), '-std=c++17', '-O2', '-Wall', '-Wextra', '-I', str(ROOT),
                str(ROOT / 'tests/geometry.cpp'), '-o', str(exe)]
        if widened:
            args.insert(1, '-DMTY_INT64')
        subprocess.run(args, check=True, timeout=180)
        subprocess.run([str(exe)], check=True, timeout=150)
        check_oracle(exe)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--smoke', action='store_true', help='also run exact/high-precision and fixed adversarial cases')
    parser.add_argument('--area', action='store_true', help='only run the shoelace area checks after compiling examples')
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='mty-geometry-') as directory:
        check(Path(directory), smoke=args.smoke)
        if args.area and not args.smoke:
            check_area(Path(directory))
