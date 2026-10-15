"""Compile tree templates; --smoke checks fixed cases, --audit adds tiny independent models."""
from pathlib import Path
import argparse
import os
import shutil
import subprocess
import tempfile
from compile_examples import ROOT, compile_examples

NAMES = ('metrics', 'centroid', 'lca', 'rmq_lca', 'difference', 'hld', 'long_chain',
         'virtual_tree', 'virtual_cut', 'cartesian', 'kruskal_tree', 'dsu_on_tree')
CHAPTERS = ('basics', 'diameter', 'center', 'centroid', 'lca', 'rmq_lca', 'hld_lca', 'difference',
            'hld', 'long_chain', 'edge_to_vertex', 'virtual_tree', 'virtual_cut',
            'cartesian', 'kruskal_tree', 'dsu_on_tree')
SOURCES = ['code/data-structures/dsu.cpp'] + [f'code/trees/{name}.cpp' for name in NAMES]


def check(temp, smoke=False, audit=False):
    compile_examples(temp, 'trees', SOURCES, [f'chapters/trees/{name}.tex' for name in CHAPTERS])
    if not (smoke or audit):
        return
    source = ['#include<bits/stdc++.h>', '#ifdef MTY_INT64', '#define int long long',
              '#endif', 'using namespace std;']
    source += [f'#include "{path}"' for path in SOURCES]
    source.append((ROOT / 'tests/trees.cpp').read_text(encoding='utf-8'))
    cpp = temp / 'trees_check.cpp'
    cpp.write_text('\n'.join(source), encoding='utf-8')
    compiler = shutil.which('g++')
    for widened in (False, True):
        exe = temp / ('trees64' if widened else 'trees32')
        if os.name == 'nt':
            exe = exe.with_suffix('.exe')
        args = [compiler, '-std=c++17', '-O2', '-Wall', '-Wextra', '-I', str(ROOT), str(cpp), '-o', str(exe)]
        if widened:
            args.insert(1, '-DMTY_INT64')
        subprocess.run(args, check=True, timeout=90)
        subprocess.run([str(exe)] + (['--audit'] if audit else []), check=True, timeout=60)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--smoke', action='store_true', help='also run fixed boundary cases')
    parser.add_argument('--audit', action='store_true', help='also compare small trees with independent models')
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='mty-trees-') as directory:
        check(Path(directory), smoke=args.smoke, audit=args.audit)
