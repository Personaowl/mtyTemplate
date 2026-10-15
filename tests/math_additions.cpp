#include<bits/stdc++.h>
#ifdef MTY_INT64
#define int long long
#endif
using namespace std;
using i64=long long;
#include "code/number-theory/qpow.cpp"
#include "code/number-theory/lagrange.cpp"
#include "code/number-theory/fft.cpp"
#include "code/number-theory/ntt.cpp"
#include "code/number-theory/prime_test.cpp"
long long checks=0;
void require(bool ok){checks++;if(!ok){cerr<<"Math additions failed #"<<checks<<'\n';exit(1);}}
long long eval(const vector<long long>& c,long long x,long long p){
	x%=p;if(x<0) x+=p;long long ans=0;
	for(int i=(int)c.size()-1;i>=0;i--) ans=(ans*x+c[i])%p;
	return ans;
}
signed main(){
	mt19937_64 rng(20261015);
	for(long long p:{2LL,17LL,101LL,998244353LL}) for(int t=0;t<80;t++){
		int n=1+rng()%min(7LL,p);vector<long long> c(n),xs(n),ys(n),y(n);
		for(auto& v:c) v=rng()%p;
		vector<long long> pool(min(100LL,p));iota(pool.begin(),pool.end(),0);
		shuffle(pool.begin(),pool.end(),rng);
		for(int i=0;i<n;i++){
			xs[i]=pool[i]-p;ys[i]=eval(c,xs[i],p);y[i]=eval(c,i,p);
			if(i&1){ys[i]-=p;y[i]-=p;}
		}
		for(long long x:{LLONG_MIN,-123LL,0LL,1LL,3LL,20LL,LLONG_MAX}){
			require(lagrange(xs,ys,x,p)==eval(c,x,p));
			require(lagrange_consecutive(y,x,p)==eval(c,x,p));
		}
	}
	for(int t=0;t<220;t++){
		int n=1+rng()%25,m=1+rng()%25;
		if(n==1&&m==1) m=2; // 直接变换不调用长度 1。
		vector<int> a(n),b(m),aa(n),bb(m),expected(n+m-1),modexpected(n+m-1);
		for(int i=0;i<n;i++){a[i]=(long long)(rng()%11)-5;aa[i]=(a[i]+NTT::Mod)%NTT::Mod;}
		for(int i=0;i<m;i++){b[i]=(long long)(rng()%11)-5;bb[i]=(b[i]+NTT::Mod)%NTT::Mod;}
		for(int i=0;i<n;i++) for(int j=0;j<m;j++) expected[i+j]+=a[i]*b[j];
		for(int i=0;i<n+m-1;i++) modexpected[i]=(expected[i]+NTT::Mod)%NTT::Mod;
		require(FFT::multiply(a,b)==expected);require(NTT::multiply(aa,bb)==modexpected);
	}
	vector<bool> prime(100001,true);prime[0]=prime[1]=false;
	for(int i=2;i*i<=100000;i++) if(prime[i]) for(int j=i*i;j<=100000;j+=i) prime[j]=false;
	for(int n=0;n<=100000;n++) require(isprime(n)==prime[n]);
	for(long long n=1;n<=4000;n++){
		long long x=n;vector<long long> expected;
		for(long long d=2;d*d<=x;d++) while(x%d==0){expected.push_back(d);x/=d;}
		if(x>1) expected.push_back(x);
		require(factorize(n)==expected);
	}
	for(auto fs:vector<vector<long long>>{{2,2,2,2,2,1000000007LL},{1000000007LL,1000000009LL},{99991,99991,99991},{3,2305843009213693951LL}}){
		__int128 n=1;for(auto p:fs) n*=p;
		if(n>=3825123056546413051LL) continue;
		require(factorize((long long)n)==fs);
	}
	// 文档记录的边界反例：保留实现确实会误判该合数，不能扩大适用范围。
	long long bound=3825123056546413051LL;
	require((__int128)149491*747451*34233211==bound);
	require(isprime(bound));
	require(isprime(2305843009213693951LL));
	cout<<"Math additions: "<<checks<<" checks passed\n";
}
