// Dependency: dinic.cpp. Edge u->v means choosing u requires choosing v.
struct ClosureResult{long long weight;vector<int> chosen;};
ClosureResult max_weight_closure(const vector<long long>& w,const vector<pair<int,int>>& edges){
	int n=(int)w.size()-1,s=n+1,t=n+2;
	long long pos=0;
	for(int u=1;u<=n;u++) if(w[u]>0) pos+=w[u];
	long long inf=pos+1;
	Dinic g;g.init(n+2);
	for(int u=1;u<=n;u++){
		if(w[u]>0) g.add(s,u,w[u]);
		else if(w[u]<0){
			// A cost above pos cannot occur in an optimal cut. Negate after widening.
			long long cap=min((__int128)inf,-(__int128)w[u]);
			g.add(u,t,cap);
		}
	}
	for(auto [u,v]:edges) g.add(u,v,inf);
	ClosureResult ans{pos-g.flow(s,t),{}};
	auto vis=g.reachable(s);
	for(int u=1;u<=n;u++) if(vis[u]) ans.chosen.push_back(u);
	return ans;
}
