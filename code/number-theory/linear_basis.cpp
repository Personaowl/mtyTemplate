struct Basis{
	static constexpr int B=60;
	array<unsigned long long,B+1> d{};
	bool zero=false;
	void init(){d.fill(0);zero=false;}
	bool insert(unsigned long long x){
		for(int i=B;i>=0;i--){
			if(!(x>>i&1)) continue;
			if(!d[i]){d[i]=x;return true;}
			x^=d[i];
		}
		zero=true;
		return false;
	}
	bool contain(unsigned long long x) const{
		for(int i=B;i>=0;i--) if(x>>i&1) x^=d[i];
		return x==0;
	}
	unsigned long long query_max(unsigned long long x=0) const{
		for(int i=B;i>=0;i--) x=max(x,x^d[i]);
		return x;
	}
	long long kth(unsigned long long k) const{
		if(!k) return -1;
		auto b=d;
		vector<unsigned long long> v;
		for(int i=0;i<=B;i++){
			if(!b[i]) continue;
			for(int j=i-1;j>=0;j--) if(b[i]>>j&1) b[i]^=b[j];
			v.push_back(b[i]);
		}
		if(zero) k--;
		if(k>=(1ULL<<v.size())) return -1;
		unsigned long long res=0;
		for(int i=0;i<(int)v.size();i++) if(k>>i&1) res^=v[i];
		return res;
	}
};
