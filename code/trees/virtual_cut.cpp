// Requires VirtualTree. Nonnegative edge costs; finite optimum must be < INF.
long long virtual_cut(VirtualTree &g,const vector<int> &keys){
	g.build(keys);if(g.key[g.root]) return -1;
	auto dfs=[&](auto &&self,int u)->long long{
		if(g.key[u]) return g.minn[u];
		__int128 sum=0;for(auto v:g.vt[u]) sum+=self(self,v);
		return min((__int128)g.minn[u],sum);
	};
	return dfs(dfs,g.root);
}
