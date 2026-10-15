"""Exercise contest scripts in temporary problem folders; never run student programs in the repository."""
from pathlib import Path
import argparse
import os
import shutil
import subprocess
import tempfile

ROOT=Path(__file__).resolve().parents[1]
GEN='#include<iostream>\n#include<string>\nint main(int argc,char**argv){std::cout<<(argc>1?std::stoll(argv[1]):1)<<"\\n";}\n'
OK='#include<iostream>\nint main(){long long x;std::cin>>x;std::cout<<x*x<<"\\n";}\n'
WA=OK.replace('x*x','x*x+1')
RE='int main(){return 3;}\n'

def check_run(bash,problem,run,env):
    def source(s): (problem/'solution file.cpp').write_text(s,encoding='utf-8')
    source(OK)
    (problem/'in.txt').write_text('7\n',encoding='utf-8')
    command=[bash,'run.sh','solution file.cpp']
    run(command,problem,0)
    assert (problem/'out.txt').read_text().strip()=='49'
    old=(problem/'out.txt').read_bytes()
    source('this is a compilation error\n')
    result=subprocess.run(command,cwd=problem,env=env,capture_output=True,timeout=60)
    assert result.returncode!=0 and (problem/'out.txt').read_bytes()==old
    source(RE);run(command,problem,3)
    print('PASS: run.sh: filename argument / fixed in.txt-out.txt / compile failure stops / exit propagation',flush=True)

def check(run_only=False):
    bash=shutil.which('bash')
    if not bash and os.name=='nt':
        candidate=Path('C:/Program Files/Git/bin/bash.exe')
        if candidate.exists():bash=str(candidate)
    if not bash:raise RuntimeError('Bash required for script checks')
    compiler=shutil.which('g++')
    if not compiler:raise RuntimeError('g++ required')
    env=dict(os.environ)
    if os.name=='nt':
        env['PATH']='C:/Program Files/Git/usr/bin;'+str(Path(compiler).parent)+';'+env['PATH']
    def run(args,cwd,expected):
        result=subprocess.run(args,cwd=cwd,env=env,capture_output=True,text=True,timeout=60)
        if result.returncode!=expected:
            raise AssertionError((args,result.returncode,result.stdout,result.stderr))
        return result
    with tempfile.TemporaryDirectory(prefix='mty-contest-') as directory:
        problem=Path(directory)/'problem with spaces';problem.mkdir()
        for name in ('stress.sh','stress.bat','run.sh'):
            shutil.copyfile(ROOT/'contest'/name,problem/name)
        for name in ('stress.sh','run.sh'):run([bash,'-n',name],problem,0)
        if run_only:
            check_run(bash,problem,run,env)
            return
        (problem/'gen.cpp').write_text(GEN,encoding='utf-8')
        (problem/'brute.cpp').write_text(OK,encoding='utf-8')
        def source(s): (problem/'main.cpp').write_text(s,encoding='utf-8')
        engines=[('Bash',[bash,'stress.sh','2','3'])]
        if os.name=='nt':engines.append(('CMD',['cmd.exe','/d','/c','stress.bat','2']))
        for name,command in engines:
            source(OK);run(command,problem,0)
            assert (problem/'stress.in').read_text().strip()=='2'
            assert (problem/'stress.seed').read_text().strip()=='2'
            source(WA);run(command,problem,1)
            assert (problem/'stress.in').read_text().strip()=='1'
            assert (problem/'stress.seed').read_text().strip()=='1'
            assert (problem/'stress.diff').stat().st_size
            source(RE);run(command,problem,2)
            source('int main(){return -1;}\n');run(command,problem,2)
            source(OK)
            (problem/'gen.cpp').write_text(RE,encoding='utf-8')
            run(command,problem,2)
            (problem/'gen.cpp').write_text(GEN,encoding='utf-8')
            (problem/'brute.cpp').write_text(RE,encoding='utf-8')
            run(command,problem,2)
            (problem/'brute.cpp').write_text(OK,encoding='utf-8')
            source('this is a compilation error\n')
            result=subprocess.run(command,cwd=problem,env=env,capture_output=True,timeout=60)
            assert result.returncode!=0,(name,result.returncode,result.stdout,result.stderr)
            print(f'PASS: {name} stress: success / mismatch / main positive-negative exits / generator-brute errors / compile failure',flush=True)
        source('int main(){for(;;){}}\n')
        run([bash,'stress.sh','1','0.2'],problem,2)
        print('PASS: Bash stress: per-program timeout',flush=True)
        check_run(bash,problem,run,env)
        exe=problem/('generator.exe' if os.name=='nt' else 'generator')
        subprocess.run([compiler,'-std=c++17','-O2',str(ROOT/'code/others/generator.cpp'),'-o',str(exe)],check=True,timeout=60)
        for seed in (1,2,123456,987654321):
            first=run([str(exe),str(seed)],problem,0).stdout
            assert first==run([str(exe),str(seed)],problem,0).stdout
            values=list(map(int,first.split()));T,n,*a=values
            assert T==1 and 1<=n<=10 and len(a)==n and all(-10<=x<=10 for x in a)
        print('PASS: actual generator: format / bounds / seed replay',flush=True)

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--run-only',action='store_true',help='only check run.sh without rerunning stress scripts')
    check(run_only=parser.parse_args().run_only)
