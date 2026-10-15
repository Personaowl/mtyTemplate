struct Hash64{
	using ull=uint64_t;
	static constexpr ull base=131;
	vector<ull> h,p;
	void init(const string &s){
		int n=s.size();h.assign(n+1,0);p.assign(n+1,1);
		for(int i=1;i<=n;i++){
			ull c=(unsigned char)s[i-1]+1;
			p[i]=p[i-1]*base;
			h[i]=h[i-1]*base+c;
		}
	}
	ull query(int l,int r){
		if(l>r) return 0;
		return h[r]-h[l-1]*p[r-l+1];
	}
	bool same(int l1,int r1,int l2,int r2){
		return r1-l1==r2-l2&&query(l1,r1)==query(l2,r2);
	}
};
