// DFS augmenting paths (Kuhn), commonly called Hungarian in Chinese CP.
struct Hungarian{
	int n,m,tim=0;
	vector<vector<int>> e;
	vector<int> ml,mr,vis;
	void init(int n,int m){this->n=n;this->m=m;e.assign(n+1,{});}
	void add(int u,int v){e[u].push_back(v);}
	bool dfs(int u){
		for(auto v:e[u]){
			if(vis[v]==tim) continue;
			vis[v]=tim;
			if(!mr[v]||dfs(mr[v])){
				ml[u]=v;mr[v]=u;return true;
			}
		}
		return false;
	}
	int solve(){
		ml.assign(n+1,0);mr.assign(m+1,0);vis.assign(m+1,0);
		tim=0;int ans=0;
		for(int u=1;u<=n;u++){tim++;ans+=dfs(u);}
		return ans;
	}
};
