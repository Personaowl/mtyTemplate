// Requires TreeLCA.
struct TreeDifference{
	TreeLCA tr;
	vector<long long> point,edge;
	void init(int n){tr.init(n);clear();}
	void add(int u,int v){tr.add(u,v);}
	void clear(){point.assign(tr.n+1,0);edge.assign(tr.n+1,0);}
	void build(int root=1){tr.build(root);clear();}
	void add_point(int u,int v,long long w){
		int p=tr.lca(u,v);
		point[u]+=w;point[v]+=w;point[p]-=w;point[tr.fa[p][0]]-=w;
	}
	void add_edge(int u,int v,long long w){
		int p=tr.lca(u,v);
		edge[u]+=w;edge[v]+=w;edge[p]-=w;edge[p]-=w;
	}
	pair<vector<long long>,vector<long long>> collect()const{
		auto a=point,b=edge;
		auto dfs=[&](auto &&self,int u,int p)->void{
			for(auto [v,w]:tr.e[u]) if(v!=p){self(self,v,u);a[u]+=a[v];b[u]+=b[v];}
		};
		dfs(dfs,tr.root,0);return {a,b};
	}
};
