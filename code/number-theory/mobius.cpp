// Requires Sieve; F[n]=sum_{d|n} f[d], with F[0] unused.
vector<__int128> mobius_invert(const vector<long long>& F,const Sieve& sv){
	int n=(int)F.size()-1;
	vector<__int128> f(n+1,0);
	for(int d=1;d<=n;d++) if(sv.mu[d]){
		for(int i=1;i<=n/d;i++) f[d*i]+=(__int128)sv.mu[d]*F[i];
	}
	return f;
}

__int128 coprime_pairs(long long n,long long m,const Sieve& sv){
	__int128 res=0;
	for(long long l=1,r;l<=min(n,m);l=r+1){
		r=min(n/(n/l),m/(m/l));
		res+=(__int128)(sv.pre_mu[r]-sv.pre_mu[l-1])*(n/l)*(m/l);
	}
	return res;
}

__int128 gcd_pairs(long long n,long long m,long long k,const Sieve& sv){
	return coprime_pairs(n/k,m/k,sv);
}
