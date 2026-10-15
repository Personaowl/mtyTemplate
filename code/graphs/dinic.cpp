struct Dinic{
	static constexpr long long INF=4000000000000000000LL;
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
	long long send(int s,int t,long long lim){
		vector<int> st(1,s),path;
		while(!st.empty()){
			int u=st.back();
			if(u==t){
				long long f=lim;
				for(auto id:path) f=min(f,e[id].cap);
				for(auto id:path){e[id].cap-=f;e[id^1].cap+=f;}
				return f;
			}
			auto &i=cur[u];
			while(i<(int)g[u].size()){
				int id=g[u][i],v=e[id].to;
				if(e[id].cap&&dep[v]==dep[u]+1) break;
				i++;
			}
			if(i==(int)g[u].size()){
				dep[u]=-1;st.pop_back();
				if(!path.empty()){path.pop_back();cur[st.back()]++;}
			}
			else{int id=g[u][i];path.push_back(id);st.push_back(e[id].to);}
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
