"""Compile DP templates/tutorials; --smoke runs targeted independent model checks."""
from pathlib import Path
import argparse
import os
import shutil
import subprocess
import tempfile
from compile_examples import ROOT, compile_examples

NAMES = ('basics', 'knapsack', 'bounded_knapsack', 'mixed_knapsack', 'group_knapsack',
         'two_cost_knapsack', 'knapsack_count', 'knapsack_trace', 'generalized_knapsack',
         'interval_merge', 'interval_ends', 'subset_dp', 'hamilton', 'profile_dp',
         'digit_dp', 'tree_knapsack', 'reroot_dp', 'probability_dp', 'expectation_dp',
         'optimization_basics', 'queue_optimization', 'stack_optimization', 'slope_basics',
         'monotone_cht', 'line_container', 'divide_conquer')
THEORY = {'basics', 'mixed_knapsack', 'generalized_knapsack', 'optimization_basics', 'slope_basics'}
SOURCES = [f'code/dp/{name}.cpp' for name in NAMES if name not in THEORY]


def check(temp, smoke=False):
    compile_examples(temp, 'dp', SOURCES, [f'chapters/dp/{name}.tex' for name in NAMES])
    if not smoke:
        return
    compiler = shutil.which('g++')
    for widened in (False, True):
        exe = temp / ('dp64' if widened else 'dp32')
        if os.name == 'nt':
            exe = exe.with_suffix('.exe')
        args = [compiler, '-std=c++17', '-O2', '-Wall', '-Wextra', '-I', str(ROOT),
                str(ROOT / 'tests/dp.cpp'), '-o', str(exe)]
        if widened:
            args.insert(1, '-DMTY_INT64')
        subprocess.run(args, check=True, timeout=120)
        subprocess.run([str(exe)], check=True, timeout=90)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--smoke', action='store_true', help='also run fixed small-model/boundary checks')
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='mty-dp-') as directory:
        check(Path(directory), smoke=args.smoke)
