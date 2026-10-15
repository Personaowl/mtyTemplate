// Requires qpow.cpp.
struct Comb{
	long long mod;
	vector<long long> fac,ifac;
	void init(int n,long long p){
		mod=p;
		fac.assign(n+1,1);
		ifac.assign(n+1,1);
		for(int i=1;i<=n;i++) fac[i]=fac[i-1]*i%mod;
		ifac[n]=qpow(fac[n],mod-2,mod);
		for(int i=n;i>=1;i--) ifac[i-1]=ifac[i]*i%mod;
	}
	long long C(long long n,long long k) const{
		if(n<0||k<0||k>n) return 0;
		return fac[n]*ifac[k]%mod*ifac[n-k]%mod;
	}
};
