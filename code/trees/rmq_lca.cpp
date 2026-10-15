struct RMQLCA{
	vector<vector<int>> e,st;
	vector<int> dep,first,ord,lg;
	void init(int n){e.assign(n+1,{});}
	void add(int u,int v){e[u].push_back(v);e[v].push_back(u);}
	int better(int u,int v)const{return dep[u]<dep[v]?u:v;}
	void build(int root=1){
		dep.assign(e.size(),0);first.assign(e.size(),0);ord.clear();
		auto dfs=[&](auto &&self,int u,int p)->void{
			dep[u]=dep[p]+1;first[u]=ord.size();ord.push_back(u);
			for(auto v:e[u]) if(v!=p){self(self,v,u);ord.push_back(u);}
		};
		dfs(dfs,root,0);int m=ord.size();lg.assign(m+1,0);
		for(int i=2;i<=m;i++) lg[i]=lg[i/2]+1;
		st.assign(lg[m]+1,vector<int>(m));st[0]=ord;
		for(int j=1;j<=lg[m];j++) for(int i=0;i+(1LL<<j)<=m;i++){
			st[j][i]=better(st[j-1][i],st[j-1][i+(1LL<<(j-1))]);
		}
	}
	int lca(int u,int v)const{
		int l=first[u],r=first[v];if(l>r) swap(l,r);
		int k=lg[r-l+1];return better(st[k][l],st[k][r-(1LL<<k)+1]);
	}
};
