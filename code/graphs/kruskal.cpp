// Requires code/data-structures/dsu.cpp.
struct Kruskal{
	struct Edge{int u,v;long long w;};
	int n;
	vector<Edge> e;
	vector<int> chosen;
	long long cost=0;
	void init(int n){this->n=n;e.clear();chosen.clear();cost=0;}
	int add(int u,int v,long long w){
		e.push_back({u,v,w});
		return (int)e.size()-1;
	}
	bool run(){
		DSU dsu;dsu.init(n);
		vector<int> id(e.size());
		iota(id.begin(),id.end(),0);
		sort(id.begin(),id.end(),[&](int x,int y){return e[x].w<e[y].w;});
		cost=0;chosen.clear();
		for(auto i:id){
			auto [u,v,w]=e[i];
			if(!dsu.merge(u,v)) continue;
			cost+=w;chosen.push_back(i);
		}
		return (int)chosen.size()==n-1;
	}
};
