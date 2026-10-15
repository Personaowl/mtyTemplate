// Requires floyd.cpp; weights must be nonnegative.
long long directed_min_cycle(int n,const vector<tuple<int,int,long long>> &e){
	Floyd g;g.init(n);
	for(auto [u,v,w]:e) g.add(u,v,w);
	g.run();
	long long ans=Floyd::INF;
	for(auto [u,v,w]:e){
		if(g.dis[v][u]==Floyd::INF) continue;
		__int128 z=(__int128)w+g.dis[v][u];
		if(z<ans) ans=z;
	}
	return ans;
}
