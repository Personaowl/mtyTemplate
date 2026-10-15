struct Sieve{
	vector<int32_t> primes,minp;
	vector<long long> phi;
	void init(int n){
		primes.clear();
		minp.assign(n+1,0);
		phi.assign(n+1,0);
		if(n>=1) phi[1]=1;
		for(int i=2;i<=n;i++){
			if(!minp[i]){
				minp[i]=i;
				primes.push_back(i);
				phi[i]=i-1;
			}
			for(auto p:primes){
				if(p>n/i) break;
				int v=i*p;
				minp[v]=p;
				if(i%p==0){
					phi[v]=phi[i]*p;
					break;
				}
				phi[v]=phi[i]*(p-1);
			}
		}
	}
	vector<unsigned long long> prime_xor(unsigned long long seed){
		mt19937_64 rng(seed);
		vector<unsigned long long> h(minp.size(),0);
		for(int i=2;i<(int)minp.size();i++){
			int p=minp[i];
			if(p==i) h[i]=rng();
			else h[i]=h[i/p]^h[p];
		}
		return h;
	}
};
