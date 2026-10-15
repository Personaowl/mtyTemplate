struct TreeCentroid{
	int n;
	vector<vector<int>> e;
	vector<int> sz,balance,centroid;
	void init(int n){this->n=n;e.assign(n+1,{});}
	void add(int u,int v){e[u].push_back(v);e[v].push_back(u);}
	void build(int root=1){
		sz.assign(n+1,0);balance.assign(n+1,0);centroid.clear();
		auto dfs=[&](auto &&self,int u,int p)->void{
			sz[u]=1;
			for(auto v:e[u]) if(v!=p){
				self(self,v,u);sz[u]+=sz[v];balance[u]=max(balance[u],sz[v]);
			}
			balance[u]=max(balance[u],n-sz[u]);
			if(balance[u]<=n/2) centroid.push_back(u);
		};
		dfs(dfs,root,0);
	}
};
