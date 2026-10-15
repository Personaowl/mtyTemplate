"""Compile current templates by default; run behavioral regression with --full."""
import argparse
from pathlib import Path
import os
import random
import shutil
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
RNG = random.Random(20261015)
PREFIX = '#include<bits/stdc++.h>\n#define int long long\nusing namespace std;\n'


def run_cpp(temp, name, source, data=''):
    compiler = shutil.which('g++')
    if not compiler:
        raise RuntimeError('g++ is required in PATH')
    cpp = temp / (name + '.cpp')
    exe = temp / (name + ('.exe' if os.name == 'nt' else ''))
    cpp.write_text(source, encoding='utf-8')
    subprocess.run([compiler, '-std=c++17', '-O2', str(cpp), '-o', str(exe)], check=True, timeout=60)
    result = subprocess.run([str(exe)], input=data, text=True, capture_output=True, check=True, timeout=15)
    return result.stdout.splitlines()


def check(temp):
    run_cpp(temp, 'solution', (ROOT / 'code/basics/solution.cpp').read_text(encoding='utf-8'), '1\n')
    cases = [(0,0,1),(0,0,7),(0,1,7),(-2,3,13),(2,0,2000000000),
             (2000000000,9223372036854775807,2000000000),
             (-9223372036854775808,17,1999999973)]
    cases += [(RNG.randint(-(2**63),2**63-1), RNG.randint(0,10**18), RNG.randint(1,2*10**9)) for _ in range(500)]
    harness = r"""signed main(){
    int a,b,mod;
    while(cin>>a>>b>>mod) cout<<qpow(a,b,mod)<<'\n';
}
"""
    source = PREFIX + (ROOT / 'code/number-theory/qpow.cpp').read_text(encoding='utf-8') + harness
    actual = run_cpp(temp, 'qpow', source, ''.join(f'{a} {b} {m}\n' for a,b,m in cases))
    expected = [str(pow(a,b,m)) for a,b,m in cases]
    if actual != expected:
        raise AssertionError('qpow results differ from Python pow')

    n = 1
    labels = list(range(n+1))
    commands = ['0 1']
    expected = []
    def reset(size):
        nonlocal n, labels
        n = size
        labels = list(range(n+1))
        commands.append(f'0 {n}')
    def op(kind,x,y=0):
        commands.append(f'{kind} {x}' + (f' {y}' if kind != 3 else ''))
        if kind == 1:
            different = labels[x] != labels[y]
            expected.append(str(int(different)))
            if different:
                old, new = labels[y], labels[x]
                labels[:] = [new if v == old else v for v in labels]
        elif kind == 2:
            expected.append(str(int(labels[x] == labels[y])))
        else:
            expected.append(str(sum(v == labels[x] for v in labels[1:])))
    op(2,1,1)
    op(1,1,1)
    op(3,1)
    reset(50)
    op(1,1,2)
    op(1,1,2)
    op(3,2)
    reset(50)
    op(2,1,2)
    op(3,1)
    for i in range(1000):
        if i % 137 == 0:
            reset(RNG.randint(1,60))
        op(RNG.randint(1,3),RNG.randint(1,n),RNG.randint(1,n))
    harness = r"""signed main(){
    DSU dsu;
    int op,x,y;
    while(cin>>op>>x){
        if(op==0) dsu.init(x);
        else if(op==3) cout<<dsu.size(x)<<'\n';
        else{
            cin>>y;
            if(op==1) cout<<dsu.merge(x,y)<<'\n';
            else cout<<dsu.check(x,y)<<'\n';
        }
    }
}
"""
    source = PREFIX + (ROOT / 'code/data-structures/dsu.cpp').read_text(encoding='utf-8') + harness
    actual = run_cpp(temp, 'dsu', source, '\n'.join(commands)+'\n')
    if actual != expected:
        raise AssertionError('DSU results differ from the naive model')
    print(f'PASS: solution, qpow ({len(cases)} cases), DSU ({len(expected)} queries)')


if __name__ == '__main__':
    from check_strings import check as check_strings
    from check_number_theory import check as check_number_theory
    from check_graphs import check as check_graphs
    from check_trees import check as check_trees
    from check_stl import check as check_stl
    from check_data_structures import check as check_data_structures
    from check_dp import check as check_dp
    from check_geometry import check as check_geometry
    from check_others import check as check_others
    from check_basic_additions import check as check_basic_additions
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--full', action='store_true', help='also run the existing behavioral regression')
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='mty-template-') as directory:
        check_strings(Path(directory), full=args.full)
        check_number_theory(Path(directory), smoke=args.full)
        check_graphs(Path(directory), smoke=args.full)
        check_trees(Path(directory), smoke=args.full)
        check_stl(Path(directory), smoke=args.full)
        check_data_structures(Path(directory), smoke=args.full)
        check_dp(Path(directory), smoke=args.full)
        check_geometry(Path(directory), smoke=args.full)
        check_others(Path(directory), smoke=args.full)
        if args.full:
            check_basic_additions(Path(directory))
            check(Path(directory))
