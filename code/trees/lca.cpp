struct TreeLCA{
	static constexpr long long INF=4e18;
	int n,lg,root,timer;
	vector<vector<pair<int,long long>>> e;
	vector<vector<int>> fa;
	vector<int> dep,dfn,out,rk,sz,bel;
	vector<long long> dis,minn;
	void init(int n){this->n=n;e.assign(n+1,{});}
	void add(int u,int v,long long w=1){e[u].push_back({v,w});e[v].push_back({u,w});}
	void build(const vector<int> &roots){
		lg=1;while((1LL<<lg)<=n) lg++;
		fa.assign(n+1,vector<int>(lg,0));
		dep.assign(n+1,0);dfn.assign(n+1,0);out.assign(n+1,0);
		rk.assign(n+1,0);sz.assign(n+1,0);bel.assign(n+1,0);
		dis.assign(n+1,0);minn.assign(n+1,INF);timer=0;
		auto dfs=[&](auto &&self,int u,int p,int rt)->void{
			fa[u][0]=p;dep[u]=dep[p]+1;bel[u]=rt;
			dfn[u]=++timer;rk[timer]=u;sz[u]=1;
			for(int j=1;j<lg;j++) fa[u][j]=fa[fa[u][j-1]][j-1];
			for(auto [v,w]:e[u]) if(v!=p){
				dis[v]=dis[u]+w;minn[v]=min(minn[u],w);
				self(self,v,u,rt);sz[u]+=sz[v];
			}
			out[u]=timer;
		};
		root=roots.empty()?0:roots[0];
		for(auto rt:roots) if(!dep[rt]) dfs(dfs,rt,0,rt);
	}
	void build(int rt=1){build(vector<int>{rt});}
	int jump(int u,long long k)const{
		if(k<0||k>=dep[u]) return 0;
		for(int j=0;j<lg;j++) if(k>>j&1) u=fa[u][j];
		return u;
	}
	int lca(int u,int v)const{
		if(!bel[u]||bel[u]!=bel[v]) return 0;
		if(dep[u]<dep[v]) swap(u,v);
		u=jump(u,dep[u]-dep[v]);if(u==v) return u;
		for(int j=lg-1;j>=0;j--) if(fa[u][j]!=fa[v][j]){
			u=fa[u][j];v=fa[v][j];
		}
		return fa[u][0];
	}
	bool ancestor(int u,int v)const{
		return bel[u]&&bel[u]==bel[v]&&dfn[u]<=dfn[v]&&dfn[v]<=out[u];
	}
	long long distance(int u,int v)const{
		int p=lca(u,v);if(!p) return INF;
		return (__int128)dis[u]+dis[v]-2*(__int128)dis[p];
	}
};
