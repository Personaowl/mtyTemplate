// Monotone optimal split positions are required; cost(l,r) is O(1).
vector<long long> divide_conquer_dp(int n,int K,
		const function<long long(int,int)>& cost){
	static constexpr long long INF=4e18;
	vector<long long> f(n+1,INF);f[0]=0;
	for(int layer=1;layer<=K;layer++){
		vector<long long> g(n+1,INF);
		auto dfs=[&](auto&& self,int l,int r,int optl,int optr)->void{
			if(l>r) return;
			int mid=(l+r)>>1,best=optl;
			for(int j=optl;j<=min(mid-1,optr);j++) if(f[j]!=INF){
				long long val=(__int128)f[j]+cost(j+1,mid);
				if(val<g[mid]){g[mid]=val;best=j;}
			}
			self(self,l,mid-1,optl,best);self(self,mid+1,r,best,optr);
		};
		dfs(dfs,layer,n,layer-1,n-1);f.swap(g);
	}
	return f;
}
