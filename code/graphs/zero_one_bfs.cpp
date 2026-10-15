struct ZeroOneBFS{
	static constexpr long long INF=4e18;
	int n;
	vector<vector<pair<int,int>>> e;
	vector<long long> dis;
	void init(int n){this->n=n;e.assign(n+1,{});dis.clear();}
	void add(int u,int v,int w){e[u].push_back({v,w});}
	void run(int s){
		dis.assign(n+1,INF);
		vector<bool> vis(n+1,false);
		deque<int> q;
		dis[s]=0;q.push_front(s);
		while(!q.empty()){
			int u=q.front();q.pop_front();
			if(vis[u]) continue;
			vis[u]=true;
			for(auto [v,w]:e[u]) if(dis[v]>dis[u]+w){
				dis[v]=dis[u]+w;
				if(w==0) q.push_front(v);
				else q.push_back(v);
			}
		}
	}
};
