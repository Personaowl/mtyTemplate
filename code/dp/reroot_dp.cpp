// ans[u] = sum of weighted distances from u to all vertices.
vector<long long> distance_sums(const vector<vector<pair<int,long long>>>& e,int root=1){
	int n=(int)e.size()-1;
	vector<int> sz(n+1);
	vector<long long> down(n+1),ans(n+1);
	auto dfs1=[&](auto&& self,int u,int p)->void{
		sz[u]=1;
		for(auto [v,w]:e[u]) if(v!=p){
			self(self,v,u);sz[u]+=sz[v];
			down[u]=(__int128)down[u]+down[v]+(__int128)sz[v]*w;
		}
	};
	auto dfs2=[&](auto&& self,int u,int p)->void{
		for(auto [v,w]:e[u]) if(v!=p){
			ans[v]=(__int128)ans[u]+(__int128)(n-2*sz[v])*w;
			self(self,v,u);
		}
	};
	dfs1(dfs1,root,0);
	ans[root]=down[root];
	dfs2(dfs2,root,0);
	return ans;
}
