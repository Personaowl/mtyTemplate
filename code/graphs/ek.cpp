// Reference: https://cp-algorithms.com/graph/edmonds_karp.html
struct EK{
	static constexpr long long INF=4e18;
	struct Edge{int to;long long cap;};
	vector<Edge> e;
	vector<vector<int>> g;
	void init(int n){g.assign(n+1,{});e.clear();}
	int add(int u,int v,long long c){
		int id=e.size();
		e.push_back({v,c});e.push_back({u,0});
		g[u].push_back(id);g[v].push_back(id^1);
		return id;
	}
	long long flow(int s,int t,long long lim=INF){
		if(s==t) return 0;
		long long ans=0;
		vector<int> pre(g.size());
		while(ans<lim){
			fill(pre.begin(),pre.end(),-1);pre[s]=-2;
			queue<int> q;q.push(s);
			while(!q.empty()&&pre[t]==-1){
				int u=q.front();q.pop();
				for(auto id:g[u]){
					int v=e[id].to;
					if(!e[id].cap||pre[v]!=-1) continue;
					pre[v]=id;q.push(v);
				}
			}
			if(pre[t]==-1) break;
			long long f=lim-ans;
			for(int v=t;v!=s;v=e[pre[v]^1].to) f=min(f,e[pre[v]].cap);
			for(int v=t;v!=s;v=e[pre[v]^1].to){
				int id=pre[v];e[id].cap-=f;e[id^1].cap+=f;
			}
			ans+=f;
		}
		return ans;
	}
	vector<bool> reachable(int s)const{
		vector<bool> vis(g.size(),false);vis[s]=true;
		queue<int> q;q.push(s);
		while(!q.empty()){
			int u=q.front();q.pop();
			for(auto id:g[u]) if(e[id].cap&&!vis[e[id].to]){
				vis[e[id].to]=true;q.push(e[id].to);
			}
		}
		return vis;
	}
};
