struct StringHash{
	static constexpr long long mod1=1000000007;
	static constexpr long long mod2=1000000009;
	static constexpr long long base1=131,base2=499;
	vector<long long> h1,h2,p1,p2;
	void init(const string &s){
		int n=s.size();
		h1.assign(n+1,0);
		h2.assign(n+1,0);
		p1.assign(n+1,1);
		p2.assign(n+1,1);
		for(int i=1;i<=n;i++){
			int c=(unsigned char)s[i-1]+1;
			p1[i]=p1[i-1]*base1%mod1;
			p2[i]=p2[i-1]*base2%mod2;
			h1[i]=(h1[i-1]*base1+c)%mod1;
			h2[i]=(h2[i-1]*base2+c)%mod2;
		}
	}
	pair<long long,long long> query(int l,int r){
		if(l>r) return {0,0};
		long long x=(h1[r]-h1[l-1]*p1[r-l+1]%mod1+mod1)%mod1;
		long long y=(h2[r]-h2[l-1]*p2[r-l+1]%mod2+mod2)%mod2;
		return {x,y};
	}
	bool same(int l1,int r1,int l2,int r2){
		if(r1-l1!=r2-l2) return false;
		return query(l1,r1)==query(l2,r2);
	}
};
