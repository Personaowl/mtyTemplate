// Requires Dinic from code/graphs/dinic.cpp.
struct FlowMatching{
	int n,m;
	vector<pair<int,int>> edges,ans;
	vector<int> coverL,coverR;
	void init(int n,int m){
		this->n=n;this->m=m;edges.clear();
		ans.clear();coverL.clear();coverR.clear();
	}
	void add(int u,int v){edges.push_back({u,v});}
	int solve(){
		int s=n+m+1,t=s+1;
		Dinic g;g.init(t);
		for(int u=1;u<=n;u++) g.add(s,u,1);
		for(int v=1;v<=m;v++) g.add(n+v,t,1);
		vector<int> id;
		for(auto [u,v]:edges) id.push_back(g.add(u,n+v,1));
		g.flow(s,t);ans.clear();
		for(int i=0;i<(int)edges.size();i++){
			if(g.e[id[i]^1].cap) ans.push_back(edges[i]);
		}
		auto vis=g.reachable(s);coverL.clear();coverR.clear();
		for(int u=1;u<=n;u++) if(!vis[u]) coverL.push_back(u);
		for(int v=1;v<=m;v++) if(vis[n+v]) coverR.push_back(v);
		return ans.size();
	}
};
