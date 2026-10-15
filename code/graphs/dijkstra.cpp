struct Dijkstra{
	static constexpr long long INF=4e18;
	vector<vector<pair<int,long long>>> e;
	vector<long long> dis;
	void init(int n){e.assign(n+1,{});dis.clear();}
	void add(int u,int v,long long w){e[u].push_back({v,w});}
	void run(int s){
		dis.assign(e.size(),INF);
		priority_queue<pair<long long,int>,vector<pair<long long,int>>,
			greater<pair<long long,int>>> q;
		dis[s]=0;q.push({0,s});
		while(!q.empty()){
			auto [d,u]=q.top();q.pop();
			if(d!=dis[u]) continue;
			for(auto [v,w]:e[u]){
				__int128 nd=(__int128)d+w;
				if(nd<dis[v]){
					dis[v]=nd;
					q.push({dis[v],v});
				}
			}
		}
	}
};
