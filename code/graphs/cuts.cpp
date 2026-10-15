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
		vector<int> fa(n+1,0),pe(n+1,-1),it(n+1,0),son(n+1,0),st;
		int tim=0;
		for(int s=1;s<=n;s++){
			if(dfn[s]) continue;
			dfn[s]=low[s]=++tim;st.push_back(s);
			while(!st.empty()){
				int u=st.back();
				if(it[u]<(int)e[u].size()){
					auto [v,id]=e[u][it[u]++];
					if(id==pe[u]) continue;
					if(!dfn[v]){
						fa[v]=u;pe[v]=id;son[u]++;
						dfn[v]=low[v]=++tim;st.push_back(v);
					}
					else low[u]=min(low[u],dfn[v]);
					continue;
				}
				st.pop_back();
				int p=fa[u];
				if(!p){cut[u]=son[u]>=2;continue;}
				low[p]=min(low[p],low[u]);
				if(low[u]>dfn[p]) bridge[pe[u]]=true;
				if(fa[p]&&low[u]>=dfn[p]) cut[p]=true;
			}
		}
		bel.assign(n+1,0);cnt=0;
		for(int s=1;s<=n;s++){
			if(bel[s]) continue;
			bel[s]=++cnt;st.push_back(s);
			while(!st.empty()){
				int u=st.back();st.pop_back();
				for(auto [v,id]:e[u]) if(!bridge[id]&&!bel[v]){
					bel[v]=cnt;st.push_back(v);
				}
			}
		}
	}
};
