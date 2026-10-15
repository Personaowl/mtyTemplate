struct PointBCC{
	int n,timer,tot,cnt;
	vector<vector<pair<int,int>>> e;
	vector<int> dfn,low;
	vector<bool> cut;
	vector<vector<int>> dcc,belong;
	void init(int n){this->n=n;tot=0;e.assign(n+1,{});}
	int add(int u,int v){
		if(u==v) return -1;
		int id=tot++;
		e[u].push_back({v,id});e[v].push_back({u,id});
		return id;
	}
	void build(){
		timer=0;cnt=0;
		dfn.assign(n+1,0);low.assign(n+1,0);cut.assign(n+1,false);
		dcc.assign(1,{});belong.assign(n+1,{});
		vector<int> stk;
		auto dfs=[&](auto &&self,int u,int pe)->void{
			dfn[u]=low[u]=++timer;stk.push_back(u);
			int son=0;
			for(auto [v,id]:e[u]){
				if(id==pe) continue;
				if(!dfn[v]){
					son++;self(self,v,id);low[u]=min(low[u],low[v]);
					if(low[v]>=dfn[u]){
						if(pe!=-1||son>1) cut[u]=true;
						vector<int> a{u};int x;
						do{x=stk.back();stk.pop_back();a.push_back(x);}while(x!=v);
						dcc.push_back(a);
					}
				}else low[u]=min(low[u],dfn[v]);
			}
			if(pe==-1&&son==0) dcc.push_back({u});
		};
		for(int u=1;u<=n;u++) if(!dfn[u]){dfs(dfs,u,-1);stk.pop_back();}
		cnt=(int)dcc.size()-1;
		for(int i=1;i<=cnt;i++) for(auto u:dcc[i]) belong[u].push_back(i);
	}
};
