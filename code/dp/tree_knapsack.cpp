// Unit cost per vertex. Selecting a vertex requires selecting all its ancestors.
vector<long long> tree_knapsack(const vector<vector<int>>& e,
	const vector<long long>& w,int K,int root=1){
	static constexpr long long INF=4e18;
	if(K==0) return {0};
	int n=(int)e.size()-1;
	vector<int> sz(n+1);
	vector<vector<long long>> f(n+1);
	auto dfs=[&](auto&& self,int u,int p)->void{
		sz[u]=1;f[u]={0,w[u]};
		for(int v:e[u]) if(v!=p){
			self(self,v,u);
			int lim=min(K,sz[u]+sz[v]);
			vector<long long> g(lim+1,-INF);g[0]=0;
			for(int i=1;i<(int)f[u].size();i++){
				for(int j=0;j<(int)f[v].size()&&i+j<=K;j++){
					if(f[u][i]==-INF||f[v][j]==-INF) continue;
					g[i+j]=max(g[i+j],(long long)((__int128)f[u][i]+f[v][j]));
				}
			}
			f[u].swap(g);sz[u]+=sz[v];
		}
	};
	dfs(dfs,root,0);
	return f[root];
}
