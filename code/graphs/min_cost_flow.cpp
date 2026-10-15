struct MinCostFlow{
	static constexpr long long INF=4000000000000000000LL;
	struct Edge{int to;long long cap,cost;};
	struct Result{bool ok;long long flow;__int128 cost;};
	vector<Edge> e;
	vector<vector<int>> g;
	void init(int n){g.assign(n+1,{});e.clear();}
	int add(int u,int v,long long cap,long long cost){
		int id=e.size();
		e.push_back({v,cap,cost});e.push_back({u,0,-cost});
		g[u].push_back(id);g[v].push_back(id^1);
		return id;
	}
	Result flow(int s,int t,long long lim=INF){
		if(s==t) return {true,0,0};
		int n=(int)g.size()-1;
		const __int128 BIG=(__int128)1<<120;
		vector<__int128> h(n+1,0),d(n+1);
		for(int k=1;k<=n;k++){
			bool changed=false;
			for(int u=1;u<=n;u++) for(auto id:g[u]){
				const auto &a=e[id];
				if(a.cap&&h[a.to]>h[u]+a.cost){
					h[a.to]=h[u]+a.cost;changed=true;
				}
			}
			if(!changed) break;
			if(k==n) return {false,0,0};
		}
		Result res{true,0,0};
		vector<int> pre(n+1);
		using Node=pair<__int128,int>;
		while(res.flow<lim){
			fill(d.begin(),d.end(),BIG);fill(pre.begin(),pre.end(),-1);
			priority_queue<Node,vector<Node>,greater<Node>> q;
			d[s]=0;q.push({0,s});
			while(!q.empty()){
				auto [du,u]=q.top();q.pop();
				if(du!=d[u]) continue;
				for(auto id:g[u]){
					const auto &a=e[id];int v=a.to;
					if(!a.cap) continue;
					__int128 nd=du+a.cost+h[u]-h[v];
					if(nd<d[v]){d[v]=nd;pre[v]=id;q.push({nd,v});}
				}
			}
			if(pre[t]==-1) break;
			for(int u=1;u<=n;u++) if(d[u]!=BIG) h[u]+=d[u];
			long long f=lim-res.flow;
			for(int v=t;v!=s;v=e[pre[v]^1].to) f=min(f,e[pre[v]].cap);
			for(int v=t;v!=s;v=e[pre[v]^1].to){
				int id=pre[v];
				e[id].cap-=f;e[id^1].cap+=f;
				res.cost+=(__int128)f*e[id].cost;
			}
			res.flow+=f;
		}
		return res;
	}
};
