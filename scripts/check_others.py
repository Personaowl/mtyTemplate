"""Compile Others examples and standalone contest programs; --smoke checks new algorithms."""
from pathlib import Path
import argparse
import os
import shutil
import subprocess
import tempfile
from compile_examples import ROOT, compile_examples

NAMES=('binary_real', 'ternary', 'meet_middle', 'prefix_2d', 'difference_2d', 'prefix_multi')

def check(temp, smoke=False):
    compile_examples(temp, 'others', [f'code/others/{n}.cpp' for n in NAMES],
                     [f'chapters/others/{n}.tex' for n in NAMES])
    compiler=shutil.which('g++')
    for name,path in [('starter','code/basics/solution.cpp'),('generator','code/others/generator.cpp')]:
        exe=temp/(name+('.exe' if os.name=='nt' else ''))
        subprocess.run([compiler,'-std=c++17','-O0','-Wall','-Wextra',str(ROOT/path),'-o',str(exe)],check=True,timeout=90)
    print('PASS: user starter and standalone data generator (standard int main)',flush=True)
    if not smoke:return
    for widened in (False,True):
        cpp=temp/'others_smoke.cpp'
        prefix=['#include<bits/stdc++.h>','#ifdef MTY_INT64','#define int long long','#endif','using namespace std;']
        prefix += [f'#include "code/others/{n}.cpp"' for n in NAMES]
        cpp.write_text('\n'.join(prefix)+'\n'+(ROOT/'tests/others.cpp').read_text(encoding='utf-8'),encoding='utf-8')
        exe=temp/('others_smoke'+str(widened)+('.exe' if os.name=='nt' else ''))
        args=[compiler,'-std=c++17','-O2','-Wall','-Wextra','-I',str(ROOT),str(cpp),'-o',str(exe)]
        if widened:args.insert(1,'-DMTY_INT64')
        subprocess.run(args,check=True,timeout=90)
        subprocess.run([str(exe)],check=True,timeout=30)

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--smoke',action='store_true')
    args=parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='mty-others-') as directory:
        check(Path(directory),smoke=args.smoke)
