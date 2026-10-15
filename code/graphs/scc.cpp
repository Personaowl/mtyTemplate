struct SCC{
	int n,cnt=0;
	vector<vector<int>> e;
	vector<int> dfn,low,bel,sz;
	void init(int n){this->n=n;e.assign(n+1,{});cnt=0;}
	void add(int u,int v){e[u].push_back(v);}
	int build(){
		dfn.assign(n+1,0);low.assign(n+1,0);bel.assign(n+1,0);
		sz.assign(1,0);cnt=0;
		vector<int> stk;
		int tim=0;
		auto dfs=[&](auto &&self,int u)->void{
			dfn[u]=low[u]=++tim;stk.push_back(u);
			for(auto v:e[u]){
				if(!dfn[v]){self(self,v);low[u]=min(low[u],low[v]);}
				else if(!bel[v]) low[u]=min(low[u],dfn[v]);
			}
			if(low[u]==dfn[u]){
				cnt++;sz.push_back(0);
				while(true){
					int v=stk.back();stk.pop_back();
					bel[v]=cnt;sz[cnt]++;
					if(v==u) break;
				}
			}
		};
		for(int u=1;u<=n;u++) if(!dfn[u]) dfs(dfs,u);
		return cnt;
	}
	vector<vector<int>> dag() const{
		vector<vector<int>> g(cnt+1);
		for(int u=1;u<=n;u++) for(auto v:e[u]){
			if(bel[u]!=bel[v]) g[bel[u]].push_back(bel[v]);
		}
		return g;
	}
};
