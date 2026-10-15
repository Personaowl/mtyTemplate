"""Compile graph examples; --smoke runs fixed cases, --audit adds tiny independent models."""
from pathlib import Path
import argparse
import os
import shutil
import subprocess
import tempfile
from compile_examples import ROOT, compile_examples

NAMES = ('dijkstra', 'spfa', 'floyd', 'kruskal', 'prim', 'layered_path', 'mod_shortest_path', 'dag_dp', 'scc', 'cuts',
         'two_sat', 'difference_constraints', 'min_cycle', 'directed_cycle', 'dinic', 'min_cost_flow')
CHAPTERS = ('dijkstra', 'spfa', 'floyd', 'mst', 'layered_path', 'mod_shortest_path', 'dag_dp', 'scc', 'cuts', 'two_sat',
            'difference_constraints', 'min_cycle', 'dinic', 'min_cost_flow', 'flow_models', 'bounded_flow')
SOURCES = ['code/data-structures/dsu.cpp'] + [f'code/graphs/{name}.cpp' for name in NAMES]


def check(temp, smoke=False, audit=False):
    compile_examples(temp, 'graphs', SOURCES, [f'chapters/graphs/{name}.tex' for name in CHAPTERS])
    if not (smoke or audit):
        return
    source = ['#include<bits/stdc++.h>', '#ifdef MTY_INT64', '#define int long long',
              '#endif', 'using namespace std;']
    source += [f'#include "{path}"' for path in SOURCES]
    source.append((ROOT / 'tests/graphs.cpp').read_text(encoding='utf-8'))
    cpp = temp / 'graphs_check.cpp'
    cpp.write_text('\n'.join(source), encoding='utf-8')
    compiler = shutil.which('g++')
    for widened in (False, True):
        exe = temp / ('graphs64' if widened else 'graphs32')
        if os.name == 'nt':
            exe = exe.with_suffix('.exe')
        args = [compiler, '-std=c++17', '-O2', '-Wall', '-Wextra', '-I', str(ROOT), str(cpp), '-o', str(exe)]
        if widened:
            args.insert(1, '-DMTY_INT64')
        subprocess.run(args, check=True, timeout=90)
        subprocess.run([str(exe)] + (['--audit'] if audit else []), check=True, timeout=45)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--smoke', action='store_true', help='also run fixed regressions')
    parser.add_argument('--audit', action='store_true', help='also compare tiny graphs with independent models')
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='mty-graphs-') as directory:
        check(Path(directory), smoke=args.smoke, audit=args.audit)
