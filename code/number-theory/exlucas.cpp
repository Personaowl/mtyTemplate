// Requires qpow.cpp, exgcd.cpp, inverse.cpp and excrt.cpp.
struct ExLucas{
	struct Part{
		long long p,q;
		vector<long long> pre;
		long long vp(long long n) const{
			long long res=0;
			while(n){n/=p;res+=n;}
			return res;
		}
		long long fac(long long n) const{
			long long res=1;
			while(n){
				res=res*qpow(pre[q],n/q,q)%q*pre[n%q]%q;
				n/=p;
			}
			return res;
		}
		long long C(long long n,long long k) const{
			long long e=vp(n)-vp(k)-vp(n-k);
			long long pw=qpow(p,e,q);
			if(!pw) return 0;
			long long res=fac(n)*inv_mod(fac(k),q)%q;
			return res*inv_mod(fac(n-k),q)%q*pw%q;
		}
	};
	long long mod;
	vector<Part> part;
	void init(long long m){
		mod=m;
		part.clear();
		auto add=[&](long long p,long long q){
			Part t;
			t.p=p,t.q=q,t.pre.assign(q+1,1);
			for(long long i=1;i<=q;i++){
				t.pre[i]=t.pre[i-1];
				if(i%p) t.pre[i]=t.pre[i]*i%q;
			}
			part.push_back(move(t));
		};
		for(long long p=2;p<=m/p;p++){
			if(m%p) continue;
			long long q=1;
			while(m%p==0){m/=p;q*=p;}
			add(p,q);
		}
		if(m>1) add(m,m);
	}
	long long C(long long n,long long k) const{
		if(k<0||k>n||mod==1) return 0;
		ExCRT crt;
		for(const auto &t:part) crt.merge(t.C(n,k),t.q);
		return crt.r;
	}
};
