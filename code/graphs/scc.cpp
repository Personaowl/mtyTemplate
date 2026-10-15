struct SCC{
	int n,cnt=0;
	vector<vector<int>> e;
	vector<int> dfn,low,bel,sz;
	void init(int n){this->n=n;e.assign(n+1,{});cnt=0;}
	void add(int u,int v){e[u].push_back(v);}
	int build(){
		dfn.assign(n+1,0);low.assign(n+1,0);bel.assign(n+1,0);
		sz.assign(1,0);cnt=0;
		vector<int> fa(n+1,0),it(n+1,0),stk,call;
		int tim=0;
		for(int s=1;s<=n;s++){
			if(dfn[s]) continue;
			dfn[s]=low[s]=++tim;stk.push_back(s);call.push_back(s);
			while(!call.empty()){
				int u=call.back();
				if(it[u]<(int)e[u].size()){
					int v=e[u][it[u]++];
					if(!dfn[v]){
						fa[v]=u;dfn[v]=low[v]=++tim;
						stk.push_back(v);call.push_back(v);
					}
					else if(!bel[v]) low[u]=min(low[u],dfn[v]);
					continue;
				}
				call.pop_back();
				if(fa[u]) low[fa[u]]=min(low[fa[u]],low[u]);
				if(low[u]==dfn[u]){
					cnt++;sz.push_back(0);
					while(true){
						int v=stk.back();stk.pop_back();
						bel[v]=cnt;sz[cnt]++;
						if(v==u) break;
					}
				}
			}
		}
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
