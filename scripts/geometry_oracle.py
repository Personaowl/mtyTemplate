"""Independent Python integer/Fraction/Decimal(100 digits) geometry reference models."""
from decimal import Decimal as D, localcontext
from fractions import Fraction as F
from itertools import combinations
import random
import subprocess


def atan_positive(x):
    scale = D(1)
    while x > D('0.1'):
        x /= 1 + (1 + x*x).sqrt()
        scale *= 2
    power, total, sign, i = x, D(0), 1, 1
    while True:
        term = power / i
        total += sign * term
        if abs(term) < D('1e-105'):
            return total * scale
        power *= x*x
        sign = -sign
        i += 2


def overlap(a, b):
    x, y, r = map(D, a)
    u, v, s = map(D, b)
    d = ((x-u)**2 + (y-v)**2).sqrt()
    pi = 16*atan_positive(D(1)/5) - 4*atan_positive(D(1)/239)
    if d >= r+s:
        return D(0)
    if d <= abs(r-s):
        return pi*min(r,s)**2
    mid = (d*d+r*r-s*s)/(2*d)
    h = max(D(0), r*r-mid*mid).sqrt()
    def angle(y, x):
        if x == 0:
            return pi/2
        a = atan_positive(y/abs(x))
        return a if x > 0 else pi-a
    return r*r*angle(h,mid)+s*s*angle(h,d-mid)-d*h


def decimal_fraction(q):
    return D(q.numerator)/q.denominator


def circle_points(a, b):
    """Independent Decimal circle subtraction, without the production Heron formula."""
    ax,ay,r=map(D,a)
    bx,by,s=map(D,b)
    dx,dy=bx-ax,by-ay
    d=(dx*dx+dy*dy).sqrt()
    x=(d*d+r*r-s*s)/(2*d)
    h=(r*r-x*x).sqrt()
    px,py=ax+dx*x/d,ay+dy*x/d
    return [(px-dy*h/d,py+dx*h/d),(px+dy*h/d,py-dx*h/d)]


def minimum_radius(points):
    if not points:
        return D(-1)
    p = [(F(x),F(y)) for x,y in points]
    candidates = [(x,y,F(0)) for x,y in p]
    for (x,y),(u,v) in combinations(p,2):
        a,b=(x+u)/2,(y+v)/2
        candidates.append((a,b,(a-x)**2+(b-y)**2))
    for (ax,ay),(bx,by),(cx,cy) in combinations(p,3):
        ux,uy,vx,vy=bx-ax,by-ay,cx-ax,cy-ay
        det=2*(ux*vy-uy*vx)
        if det == 0:
            continue
        x=((ux*ux+uy*uy)*vy-(vx*vx+vy*vy)*uy)/det
        y=(ux*(vx*vx+vy*vy)-vx*(ux*ux+uy*uy))/det
        candidates.append((ax+x,ay+y,x*x+y*y))
    best=min(r for x,y,r in candidates if all((x-u)**2+(y-v)**2<=r for u,v in p))
    return decimal_fraction(best).sqrt()


def halfplane_area(hp, bound):
    p=[(F(-bound),F(-bound)),(F(bound),F(-bound)),(F(bound),F(bound)),(F(-bound),F(bound))]
    for a,b,c in hp:
        q=[]
        for i,(x,y) in enumerate(p):
            u,v=p[(i+1)%len(p)]
            s,t=a*x+b*y+c,a*u+b*v+c
            if s>=0:
                q.append((x,y))
            if (s>=0)!=(t>=0):
                k=s/(s-t)
                q.append((x+(u-x)*k,y+(v-y)*k))
        p=q
    area=sum((x*p[(i+1)%len(p)][1]-y*p[(i+1)%len(p)][0] for i,(x,y) in enumerate(p)),F(0))
    return decimal_fraction(abs(area)/2)


def check(exe):
    rng=random.Random(20261015)
    cases=[]
    def add(text, expected, kind, data=None):
        cases.append((text,expected,kind,data))
    with localcontext() as context:
        context.prec=100
        for _ in range(1000):
            ax,ay,bx,by,cx,cy=[rng.randint(-10**12,10**12) for _ in range(6)]
            want=(bx-ax)*(cy-ay)-(by-ay)*(cx-ax)
            add('0 '+' '.join(map(str,(ax,ay,bx,by,cx,cy))),want,'integer')
        pairs=[((0,0,1),(2,0,1)),((0,0,1),('1.99999999',0,1)),((0,0,1),('2.00000001',0,1)),
               ((0,0,2),(1,0,1)),((0,0,2),('1.00000001',0,1)),((0,0,2),('0.99999999',0,1)),
               ((0,0,0),(0,0,0)),((0,0,1),(0,0,2))]
        for _ in range(260):
            pairs.append(((rng.randint(-30,30),rng.randint(-30,30),rng.randint(0,20)),
                          (rng.randint(-30,30),rng.randint(-30,30),rng.randint(0,20))))
        for a,b in pairs:
            add('1 '+' '.join(map(str,(*a,*b))),overlap(a,b),'overlap')
        # Binary-exact inputs isolate arithmetic loss from decimal input rounding.
        for large in (-19,-10,0,10,19):
            for small in (-19,-10,0,10,19):
                if small>large:
                    continue
                R,r=D(2)**large,D(2)**small
                for t in (D(1)/256,D(1)/16,D(1)/2,D(15)/16,D(255)/256,1-D(2)**-20):
                    d=R-r+2*r*t
                    for dx,dy in ((d,0),(0,d),(-d,0),(0,-d)):
                        for a,b in (((0,0,R),(dx,dy,r)),((dx,dy,r),(0,0,R))):
                            values=' '.join(map(str,(*a,*b)))
                            add('1 '+values,overlap(a,b),'overlap_scaled',r)
                            if min(d-abs(R-r),R+r-d)>D('2e-10'):
                                add('4 '+values,circle_points(a,b),'circle_points')
        sets=[[],[(0,0)],[(0,0),(0,0)],[(0,0),(1,0),(2,0)],[(0,0),(4,0),(0,3)],
              [(10**9,10**9),(10**9+4,10**9),(10**9,10**9+3)]]
        for _ in range(260):
            sets.append([(rng.randint(-15,15),rng.randint(-15,15)) for _ in range(rng.randint(0,9))])
        for p in sets:
            add('2 '+str(len(p))+' '+' '.join(f'{x} {y}' for x,y in p),minimum_radius(p),'mec',p)
        support_sets=[[(0,0),(4,0),(0,3),(1,1)],
                      [(0,0),(1,D(2)**-30),(2,0),(1,-D(2)**-30)],
                      [(0,0),(2,0),(1,2),(1,1),(1,1)],
                      [(0,0),(0,0),(0,0)],[(0,0),(1,0),(3,0),(2,0)]]
        for _ in range(8):
            support_sets.append([(rng.randint(-3,3),rng.randint(-3,3)) for _ in range(8)])
        for scale in (D(2)**-19,D(1),D(2)**16):
            for ox,oy in ((D(0),D(0)),(D(2)**19,-D(2)**19)):
                for points in support_sets:
                    p=[(ox+D(x)*scale,oy+D(y)*scale) for x,y in points]
                    add('2 '+str(len(p))+' '+' '.join(f'{x} {y}' for x,y in p),minimum_radius(p),'mec_scaled',p)
        for _ in range(700):
            hp=[]
            for _ in range(rng.randint(0,16)):
                a,b=rng.randint(-8,8),rng.randint(-8,8)
                if a or b:
                    hp.append((a,b,rng.randint(-25,25)))
            text='3 '+str(len(hp))+' 30 '+' '.join(' '.join(map(str,h)) for h in hp)
            want=halfplane_area(hp,30)
            add(text,want,'halfplane')
            add('5'+text[1:],want,'clipping')
        for Q in (2**10,2**30,2**50,2**60):
            hp=[(Q,1,Q),(-Q,-1,Q),(0,Q,Q),(0,-Q,Q),
                (Q,Q+1,3*Q),(-Q,-Q+1,3*Q)]
            for rotation in range(4):
                for repeat in range(4):
                    a=hp*(1+repeat%2)
                    rng.shuffle(a)
                    text='3 '+str(len(a))+' 32 '+' '.join(' '.join(map(str,h)) for h in a)
                    want=halfplane_area(a,32)
                    add(text,want,'halfplane')
                    add('5'+text[1:],want,'clipping')
                hp=[(b,-a,c) for a,b,c in hp]
        result=subprocess.run([str(exe),'--oracle'],input='\n'.join(c[0] for c in cases)+'\n',
                              text=True,capture_output=True,check=True,timeout=90)
        actual=result.stdout.splitlines()
        if len(actual)!=len(cases):
            raise AssertionError(f'geometry oracle output count: {len(actual)} vs {len(cases)}')
        for line,(text,want,kind,data) in zip(actual,cases):
            if kind=='integer':
                if int(line)!=want:
                    raise AssertionError(f'Python integer mismatch: {text}: {line} vs {want}')
                continue
            fields=list(map(D,line.split()))
            if kind=='circle_points':
                if fields[0]!=2 or len(fields)!=5:
                    raise AssertionError(f'proper circle intersection misclassified: {text}: {line}')
                got=[(fields[1],fields[2]),(fields[3],fields[4])]
                error=min(max(((x-u)**2+(y-v)**2).sqrt() for (x,y),(u,v) in zip(got,order))
                          for order in (want,want[::-1]))
                if error>D('2e-10'):
                    raise AssertionError(f'circle intersection Decimal mismatch: {text}: {line}: {error}')
                continue
            got=fields[-1]
            if kind=='overlap_scaled':
                tol=D('5e-11')*abs(want)+D('1e-18')*data*data
                if abs(got-want)>tol:
                    raise AssertionError(f'scaled circle overlap mismatch: {text}: {line} vs {want}, tol={tol}')
                continue
            tol=D('2e-11') if kind=='mec' else D('3e-12')
            if kind=='mec_scaled':
                tol=D('2e-10')/(1+abs(want))
            if abs(got-want)>tol*(1+abs(want)):
                raise AssertionError(f'{kind} oracle mismatch: {text}: {line} vs {want}')
            if kind in ('mec','mec_scaled') and data:
                x,y,r=fields
                if any(((D(a)-x)**2+(D(b)-y)**2).sqrt()>r+D('2e-10') for a,b in data):
                    raise AssertionError(f'MEC returned circle misses input: {text}: {line}')
        print(f'PASS: geometry Python oracle: {len(cases)} arbitrary-integer/Fraction/Decimal(100) cases',flush=True)
