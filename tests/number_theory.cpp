// Included after the template sources by scripts/check_number_theory.py --smoke.
int checks=0;
void require(bool ok,const char *name){
	checks++;
	if(!ok){cerr<<"FAIL: "<<name<<'\n';exit(1);}
}
signed main(){
	require(qpow(2,10,1000)==24,"qpow full loop");
	require(qpow(-2,3,13)==5&&qpow(0,0,1)==0,"qpow normalization");
	long long x,y,d=exgcd(18,30,x,y);
	require(d==6&&18*x+30*y==d,"exgcd back substitution");
	d=exgcd(-18,30,x,y);
	require(d==6&&-18*x+30*y==d,"signed exgcd");
	d=exgcd(-7,0,x,y);
	require(d==7&&-7*x==d,"zero coefficient");
	require(inv_mod(3,10)==7&&inv_mod(6,10)==-1,"inverse existence");
	auto inv=inverse_table(10,101);
	bool ok=true;
	for(int i=1;i<=10;i++) ok&=i*inv[i]%101==1;
	require(ok,"inverse table");
	Sieve sv;
	sv.init(100);
	require(sv.primes.size()==25&&sv.minp[84]==2&&sv.phi[36]==12,"linear sieve");
	auto h=sv.prime_xor(20261015);
	require(h[1]==0&&h[12]==h[3]&&h[36]==0,"prime parity fingerprint");
	sv.init(1);
	require(sv.primes.empty()&&sv.phi[1]==1,"sieve reinitialization");
	require(phi(1)==1&&phi(36)==12,"totient");
	require(pow_decimal(2,"2",8)==4,"noncoprime small exponent");
	require(pow_decimal(2,"4",8)==0,"noncoprime large exponent");
	require(pow_decimal(6,"0000",8)==1&&pow_decimal(6,"0",1)==0,"decimal zero exponent");
	require(vp_fact(10,2)==8&&vp_fact(0,3)==0,"Legendre");
	require(remainder_sum(5,3)==7&&remainder_sum(0,10)==0&&remainder_sum(5,0)==0,"division blocks");
	Comb cb;
	cb.init(20,101);
	require(cb.C(10,3)==19&&cb.C(0,0)==1&&cb.C(3,4)==0,"factorial combinations");
	Lucas lc;
	lc.init(7);
	require(lc.C(100,20)==0&&lc.C(10,3)==1,"Lucas digits");
	ExCRT crt;
	require(crt.merge(2,6)==1&&crt.merge(5,9)==1&&crt.r==14&&crt.m==18,"noncoprime CRT");
	require(crt.merge(0,3)==0&&crt.r==14&&crt.m==18,"inconsistent CRT");
	crt.init();
	require(crt.merge(0,LLONG_MAX)==1&&crt.merge(0,2)==-1,"CRT overflow status");
	crt.init();
	require(crt.merge(-1,5)==1&&crt.merge(0,1)==1&&crt.r==4,"CRT normalization");
	ExLucas ex;
	ex.init(72);
	require(ex.C(10,3)==48&&ex.C(8,4)==70&&ex.C(10,0)==1,"prime power binomial");
	ex.init(1);
	require(ex.C(10,3)==0,"modulus one");
	vector<int> a={0,3,1,2};
	require(cantor_rank(a,998244353)==5&&cantor_rank(a,5)==0,"Cantor rank normalization");
	Mat m(2,1000);
	m.a[1][1]=1,m.a[1][2]=1,m.a[2][1]=1;
	auto r=m.pow(5),unit=m.pow(0);
	require(r.a[1][1]==8&&r.a[1][2]==5&&unit.a[1][1]==1&&unit.a[1][2]==0,"matrix power");
	MaxMat mm(2);
	mm.a[1][1]=0,mm.a[1][2]=5,mm.a[2][2]=2;
	auto rr=mm.pow(3);
	require(rr.a[1][2]==9&&rr.a[2][1]==MaxMat::INF,"max-plus unreachable");
	auto seg=[](long long v){
		MaxMat t(3);
		t.a[1][1]=0,t.a[1][2]=t.a[1][3]=v;
		t.a[2][2]=t.a[2][3]=v,t.a[3][3]=0;
		return t;
	};
	auto best=seg(-5)*seg(-2)*seg(-7);
	require(best.a[1][3]==-2,"nonempty maximum subarray");
	vector<double> ans;
	int status=gauss({{1,1,3},{2,-1,0}},ans);
	require(status==1&&abs(ans[0]-1)<1e-8&&abs(ans[1]-2)<1e-8,"unique real system");
	status=gauss({{0,1,2},{0,2,4}},ans);
	require(status==2&&ans[0]==0&&abs(ans[1]-2)<1e-8,"free leading variable");
	require(gauss({{1,1},{1,2}},ans)==0,"inconsistent real system");
	vector<int> bits;
	int rank;
	status=xor_gauss({{0,1,1},{0,1,1}},bits,rank);
	require(status==2&&rank==1&&bits[0]==0&&bits[1]==1,"xor rank");
	require(xor_gauss({{1,0},{1,1}},bits,rank)==0,"xor contradiction");
	Basis lb;
	lb.insert(3),lb.insert(5);
	require(lb.query_max()==6&&lb.kth(1)==3&&lb.kth(3)==6&&lb.kth(4)==-1,"independent basis");
	lb.insert(6);
	require(lb.zero&&lb.kth(1)==0&&lb.kth(4)==6&&lb.contain(0)&&!lb.contain(7),"dependent basis");
	lb.init();
	lb.insert(1ULL<<60);
	require(lb.kth(1)==(1LL<<60)&&lb.kth(0)==-1,"basis bit limit");
	// Fixed boundary review; large binomial residues computed with Python math.comb.
	require(qpow(LLONG_MIN,1,2000000000)==1145224192,"minimum signed base");
	bool coeff_ok=true;
	for(auto [a,b]:vector<pair<long long,long long>>{
		{LLONG_MAX,LLONG_MAX-1},{-LLONG_MAX,LLONG_MAX-1},
		{-LLONG_MAX,-(LLONG_MAX-1)},{0,LLONG_MAX}}){
		long long u,v,g=exgcd(a,b,u,v);
		coeff_ok&=g==gcd(a,b)&&(__int128)a*u+(__int128)b*v==g;
	}
	require(coeff_ok,"large signed Bezout coefficients");
	ex.init(72);
	require(ex.C(1000000000000000123LL,20)==18,"large binomial mixed modulus");
	ex.init(1024);
	require(ex.C(1000000000000000123LL,7)==854,"large binomial power of two");
	ex.init(3125);
	require(ex.C(1000000000000000123LL,20)==646,"large binomial odd prime power");
	ex.init(77);
	require(ex.C(1000000000000000123LL,17)==66,"large binomial squarefree modulus");
	sv.init(0);
	require(sv.primes.empty()&&sv.minp.size()==1,"empty sieve");
	lb.init();
	for(int i=0;i<=60;i++) lb.insert(1ULL<<i);
	require(lb.kth((1ULL<<61)-1)==(1LL<<61)-1&&lb.kth(1ULL<<61)==-1,"full basis rank");
	lb.insert(0);
	require(lb.kth(1)==0&&lb.kth(1ULL<<61)==(1LL<<61)-1,"full rank nonempty zero");
	SG game;
	game.init(0);
	require(game.build()&&game.sg[0]==0,"SG terminal state");
	game.init(8);
	for(int x=1;x<=8;x++){
		for(auto d:vector<int>{1,3,4}) if(x>=d) game.add(x,x-d);
	}
	vector<int> expected={0,1,0,1,2,3,2,0,1};
	require(game.build()&&game.sg==expected,"SG subtraction game");
	game.init(5);
	game.add(5,1),game.add(1,3),game.add(5,2);
	game.add(2,4),game.add(4,3),game.add(5,1);
	require(game.build()&&game.sg[5]==2&&game.sg[2]==0&&game.build(),"SG topology and duplicate edges");
	game.add(3,2);
	require(!game.build(),"SG cycle rejection");
	game.init(2);
	require(game.build()&&game.sg==vector<int>({0,0,0}),"SG reinitialization");
	cout<<"PASS: number theory fixed cases ("<<checks<<")\n";
}
