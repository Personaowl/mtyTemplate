// Requires qpow.cpp and comb.cpp.
struct Lucas{
	long long p;
	Comb cb;
	void init(int mod){
		p=mod;
		cb.init(p-1,p);
	}
	long long C(long long n,long long k) const{
		if(k<0||k>n) return 0;
		long long res=1;
		while(n||k){
			res=res*cb.C(n%p,k%p)%p;
			n/=p,k/=p;
		}
		return res;
	}
};
