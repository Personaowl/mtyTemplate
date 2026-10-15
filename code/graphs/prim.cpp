struct Prim{
	vector<vector<pair<int,long long>>> e;
	long long cost=0;
	void init(int n){e.assign(n+1,{});cost=0;}
	void add(int u,int v,long long w){
		e[u].push_back({v,w});e[v].push_back({u,w});
	}
	bool run(int s=1){
		int n=(int)e.size()-1,cnt=0;
		vector<bool> vis(n+1,false);
		priority_queue<pair<long long,int>,vector<pair<long long,int>>,
			greater<pair<long long,int>>> q;
		q.push({0,s});cost=0;
		while(!q.empty()){
			auto [w,u]=q.top();q.pop();
			if(vis[u]) continue;
			vis[u]=true;cnt++;cost+=w;
			for(auto [v,c]:e[u]) if(!vis[v]) q.push({c,v});
		}
		return cnt==n;
	}
};
