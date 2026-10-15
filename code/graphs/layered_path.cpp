struct DiscountPath{
	static constexpr long long INF=4000000000000000000LL;
	int k;
	vector<vector<pair<int,long long>>> e;
	vector<vector<long long>> dis;
	void init(int n,int k){this->k=k;e.assign(n+1,{});dis.clear();}
	void add(int u,int v,long long w){e[u].push_back({v,w});}
	void run(int s){
		dis.assign(e.size(),vector<long long>(k+1,INF));
		using Node=tuple<long long,int,int>;
		priority_queue<Node,vector<Node>,greater<Node>> q;
		dis[s][0]=0;q.push({0,s,0});
		while(!q.empty()){
			auto [d,u,h]=q.top();q.pop();
			if(d!=dis[u][h]) continue;
			for(auto [v,w]:e[u]){
				auto relax=[&](int t,long long c){
					__int128 nd=(__int128)d+c;
					if(nd<dis[v][t]){
						dis[v][t]=nd;q.push({dis[v][t],v,t});
					}
				};
				relax(h,w);
				if(h<k) relax(h+1,w/2);
			}
		}
	}
	long long query(int t) const{
		return *min_element(dis[t].begin(),dis[t].end());
	}
};
