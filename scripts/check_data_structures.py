"""Compile data structure templates and tutorials; --smoke runs targeted checks."""
from pathlib import Path
import argparse
import os
import shutil
import subprocess
import tempfile
from compile_examples import ROOT, compile_examples

NAMES = ('dsu', 'weighted_dsu', 'kind_dsu', 'rollback_dsu',
         'monotonic_stack', 'monotonic_queue', 'median_heap', 'binary_trie', 'st', 'bit',
         'bit_distinct', 'bit_2d_count', 'seg_point', 'seg_lazy',
         'seg_affine', 'seg_max_subarray', 'seg_equal_run', 'seg_dynamic', 'li_chao',
         'hld_segment', 'time_segment', 'persistent_array', 'persistent_kth', 'odt', 'sqrt_block', 'mo', 'mo_update')
SOURCES = [f'code/data-structures/{name}.cpp' for name in NAMES if name != 'hld_segment']
SOURCES += ['code/data-structures/binary_dsu.cpp']
SOURCES += ['code/trees/hld.cpp', 'code/data-structures/hld_segment.cpp']


def check_time_segment(temp):
    for widened in (False, True):
        exe = temp / ('time_segment64' if widened else 'time_segment32')
        if os.name == 'nt':
            exe = exe.with_suffix('.exe')
        args = [shutil.which('g++'), '-std=c++17', '-O2', '-Wall', '-Wextra', '-I', str(ROOT),
                str(ROOT / 'tests/time_segment.cpp'), '-o', str(exe)]
        if widened:
            args.insert(1, '-DMTY_INT64')
        subprocess.run(args, check=True, timeout=90)
        subprocess.run([str(exe)], check=True, timeout=60)


def check(temp, smoke=False, time_segment=False):
    compile_examples(temp, 'data_structures', SOURCES,
                     [f'chapters/data-structures/{name}.tex' for name in NAMES])
    if smoke or time_segment:
        check_time_segment(temp)
    if not smoke:
        return
    compiler = shutil.which('g++')
    for widened in (False, True):
        exe = temp / ('ds64' if widened else 'ds32')
        if os.name == 'nt':
            exe = exe.with_suffix('.exe')
        args = [compiler, '-std=c++17', '-O2', '-Wall', '-Wextra', '-I', str(ROOT),
                str(ROOT / 'tests/data_structures.cpp'), '-o', str(exe)]
        if widened:
            args.insert(1, '-DMTY_INT64')
        subprocess.run(args, check=True, timeout=90)
        subprocess.run([str(exe)], check=True, timeout=60)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--smoke', action='store_true', help='also run targeted data structure checks')
    parser.add_argument('--time-segment', action='store_true', help='also run only time-segment bipartite models')
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='mty-ds-') as directory:
        check(Path(directory), smoke=args.smoke, time_segment=args.time_segment)
