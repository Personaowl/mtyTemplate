"""Compile number theory by default; --smoke also runs a small set of fixed cases."""
from pathlib import Path
import argparse
import os
import shutil
import subprocess
import tempfile
from compile_examples import ROOT, compile_examples

# Dependencies precede consumers. Keep this list in sync with newly added code entries.
NAMES = ('qpow', 'exgcd', 'inverse', 'sieve', 'mobius', 'euler', 'legendre', 'division_blocks',
         'comb', 'lucas', 'excrt', 'exlucas', 'cantor', 'matrix', 'max_matrix',
         'gauss', 'xor_gauss', 'linear_basis', 'sg', 'lagrange', 'fft', 'ntt', 'prime_test')
CHAPTERS = ('qpow', 'exgcd', 'inverse', 'sieve', 'mobius', 'euler', 'legendre', 'division_blocks',
            'comb', 'counting', 'lucas', 'cantor', 'inclusion', 'matrix', 'gauss',
            'excrt', 'linear_basis', 'games', 'lagrange', 'fft', 'ntt', 'prime_test')

def check_additions(temp):
    for widened in (False, True):
        exe=temp/('math_additions'+str(widened)+('.exe' if os.name=='nt' else ''))
        args=[shutil.which('g++'),'-std=c++17','-O2','-Wall','-Wextra','-I',str(ROOT),
              str(ROOT/'tests/math_additions.cpp'),'-o',str(exe)]
        if widened:args.insert(1,'-DMTY_INT64')
        subprocess.run(args,check=True,timeout=90)
        subprocess.run([str(exe)],check=True,timeout=45)

def check(temp, smoke=False):
    compile_examples(temp, 'number_theory',
                     [f'code/number-theory/{name}.cpp' for name in NAMES],
                     [f'chapters/number-theory/{name}.tex' for name in CHAPTERS],
                     preamble=['using i64=long long;'])
    if not smoke:
        return
    check_additions(temp)
    compiler = shutil.which('g++')
    cpp = temp / 'number_theory_smoke.cpp'
    source = ['#include<bits/stdc++.h>', '#ifdef MTY_INT64', '#define int long long',
              '#endif', 'using namespace std;', 'using i64=long long;']
    source += [f'#include "code/number-theory/{name}.cpp"' for name in NAMES]
    source.append((ROOT / 'tests/number_theory.cpp').read_text(encoding='utf-8'))
    cpp.write_text('\n'.join(source), encoding='utf-8')
    for widened in (False, True):
        exe = temp / ('math_smoke64' if widened else 'math_smoke32')
        if os.name == 'nt':
            exe = exe.with_suffix('.exe')
        args = [compiler, '-std=c++17', '-O2', '-Wall', '-Wextra', '-I', str(ROOT), str(cpp), '-o', str(exe)]
        if widened:
            args.insert(1, '-DMTY_INT64')
        subprocess.run(args, check=True, timeout=90)
        subprocess.run([str(exe)], check=True, timeout=15)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--smoke', action='store_true', help='also run fixed boundary and regression cases')
    parser.add_argument('--additions', action='store_true', help='only run interpolation/convolution/primality additions after compiling examples')
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='mty-math-') as directory:
        check(Path(directory), smoke=args.smoke)
        if args.additions and not args.smoke:
            check_additions(Path(directory))
