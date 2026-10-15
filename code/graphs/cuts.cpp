struct Cuts{
	int n,cnt=0;
	vector<vector<pair<int,int>>> e;
	vector<pair<int,int>> edges;
	vector<int> dfn,low,bel;
	vector<bool> cut,bridge;
	void init(int n){this->n=n;e.assign(n+1,{});edges.clear();cnt=0;}
	int add(int u,int v){
		int id=edges.size();edges.push_back({u,v});
		e[u].push_back({v,id});e[v].push_back({u,id});
		return id;
	}
	void build(){
		dfn.assign(n+1,0);low.assign(n+1,0);cut.assign(n+1,false);
		bridge.assign(edges.size(),false);
		int tim=0;
		auto dfs=[&](auto &&self,int u,int pe)->void{
			dfn[u]=low[u]=++tim;int son=0;
			for(auto [v,id]:e[u]){
				if(id==pe) continue;
				if(!dfn[v]){
					son++;self(self,v,id);low[u]=min(low[u],low[v]);
					if(low[v]>dfn[u]) bridge[id]=true;
					if(pe!=-1&&low[v]>=dfn[u]) cut[u]=true;
				}
				else low[u]=min(low[u],dfn[v]);
			}
			if(pe==-1) cut[u]=son>=2;
		};
		for(int u=1;u<=n;u++) if(!dfn[u]) dfs(dfs,u,-1);
		bel.assign(n+1,0);cnt=0;
		auto paint=[&](auto &&self,int u)->void{
			bel[u]=cnt;
			for(auto [v,id]:e[u]) if(!bridge[id]&&!bel[v]){
				self(self,v);
			}
		};
		for(int u=1;u<=n;u++) if(!bel[u]){cnt++;paint(paint,u);}
	}
};
