struct TreeMetrics{
	static constexpr long long INF=4e18;
	int n,a,b;
	long long diameter;
	vector<vector<pair<int,long long>>> e;
	vector<long long> da,db;
	vector<int> path,center;
	void init(int n){this->n=n;e.assign(n+1,{});}
	void add(int u,int v,long long w=1){e[u].push_back({v,w});e[v].push_back({u,w});}
	int walk(int s,vector<long long> &d,vector<int> &fa){
		d.assign(n+1,0);fa.assign(n+1,0);int far=s;
		auto dfs=[&](auto &&self,int u,int p)->void{
			fa[u]=p;if(d[u]>d[far]) far=u;
			for(auto [v,w]:e[u]) if(v!=p){d[v]=d[u]+w;self(self,v,u);}
		};
		dfs(dfs,s,0);return far;
	}
	void build(){
		vector<long long> d;vector<int> fa;
		a=walk(1,d,fa);b=walk(a,da,fa);
		path.clear();for(int u=b;u;u=fa[u]) path.push_back(u);
		reverse(path.begin(),path.end());walk(b,db,fa);
		diameter=da[b];long long best=INF;
		for(int u=1;u<=n;u++) best=min(best,max(da[u],db[u]));
		center.clear();
		for(int u=1;u<=n;u++) if(max(da[u],db[u])==best) center.push_back(u);
	}
};
