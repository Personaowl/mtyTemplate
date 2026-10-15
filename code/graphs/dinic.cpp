struct Dinic{
	static constexpr long long INF=4e18;
	struct Edge{int to;long long cap;};
	vector<Edge> e;
	vector<vector<int>> g;
	vector<int> dep,cur;
	void init(int n){g.assign(n+1,{});e.clear();}
	int add(int u,int v,long long c){
		int id=e.size();
		e.push_back({v,c});e.push_back({u,0});
		g[u].push_back(id);g[v].push_back(id^1);
		return id;
	}
	bool bfs(int s,int t){
		dep.assign(g.size(),-1);
		queue<int> q;q.push(s);dep[s]=0;
		while(!q.empty()){
			int u=q.front();q.pop();
			for(auto id:g[u]){
				int v=e[id].to;
				if(e[id].cap&&dep[v]==-1){dep[v]=dep[u]+1;q.push(v);}
			}
		}
		return dep[t]!=-1;
	}
	long long send(int u,int t,long long lim){
		if(u==t) return lim;
		for(int &i=cur[u];i<(int)g[u].size();i++){
			int id=g[u][i],v=e[id].to;
			if(!e[id].cap||dep[v]!=dep[u]+1) continue;
			long long f=send(v,t,min(lim,e[id].cap));
			if(!f) continue;
			e[id].cap-=f;e[id^1].cap+=f;
			return f;
		}
		return 0;
	}
	long long flow(int s,int t,long long lim=INF){
		if(s==t) return 0;
		long long ans=0;
		while(ans<lim&&bfs(s,t)){
			cur.assign(g.size(),0);
			while(ans<lim){
				long long f=send(s,t,lim-ans);
				if(!f) break;
				ans+=f;
			}
		}
		return ans;
	}
	vector<bool> reachable(int s) const{
		vector<bool> vis(g.size(),false);
		queue<int> q;q.push(s);vis[s]=true;
		while(!q.empty()){
			int u=q.front();q.pop();
			for(auto id:g[u]) if(e[id].cap&&!vis[e[id].to]){
				vis[e[id].to]=true;q.push(e[id].to);
			}
		}
		return vis;
	}
};
