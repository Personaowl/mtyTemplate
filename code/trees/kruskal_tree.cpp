// Requires DSU and TreeLCA. The reconstructed structure may be a forest.
struct KruskalTree{
	int n,tot;
	vector<tuple<int,int,long long>> edges;
	vector<long long> val;
	vector<int> sz,roots;
	TreeLCA tr;
	void init(int n){this->n=n;edges.clear();}
	void add(int u,int v,long long w){edges.push_back({u,v,w});}
	void build(){
		DSU d;d.init(n);vector<int> rt(n+1);iota(rt.begin(),rt.end(),0);
		val.assign(2*n+1,0);sz.assign(2*n+1,1);tot=n;
		vector<pair<int,int>> tree;auto a=edges;
		sort(a.begin(),a.end(),[](auto x,auto y){return get<2>(x)<get<2>(y);});
		for(auto [u,v,w]:a){
			int x=d.find(u),y=d.find(v);if(x==y) continue;
			++tot;val[tot]=w;sz[tot]=sz[rt[x]]+sz[rt[y]];
			tree.push_back({tot,rt[x]});tree.push_back({tot,rt[y]});
			d.merge(x,y);rt[d.find(x)]=tot;
		}
		roots.clear();for(int u=1;u<=n;u++) if(d.find(u)==u) roots.push_back(rt[u]);
		tr.init(tot);for(auto [u,v]:tree) tr.add(u,v);
		tr.build(roots);
	}
	pair<bool,long long> bottleneck(int u,int v)const{
		int p=tr.lca(u,v);if(!p) return {false,0};
		return {true,u==v?0:val[p]};
	}
	int component(int u,long long x)const{
		for(int j=tr.lg-1;j>=0;j--){
			int p=tr.fa[u][j];if(p&&val[p]<=x) u=p;
		}
		return u;
	}
	int size(int u,long long x)const{return sz[component(u,x)];}
};
