// Two recursive DFS passes, following the user's decomposition convention.
struct HLD{
	struct Part{int l,r;bool rev;};
	int n,root,timer;
	vector<vector<int>> e;
	vector<int> fa,dep,sz,son,top,dfn,rk;
	void init(int n){this->n=n;e.assign(n+1,{});}
	void add(int u,int v){e[u].push_back(v);e[v].push_back(u);}
	void dfs1(int u,int p){
		fa[u]=p;dep[u]=dep[p]+1;sz[u]=1;
		for(auto v:e[u]) if(v!=p){
			dfs1(v,u);sz[u]+=sz[v];if(sz[v]>sz[son[u]]) son[u]=v;
		}
	}
	void dfs2(int u,int t){
		top[u]=t;dfn[u]=++timer;rk[timer]=u;
		if(son[u]) dfs2(son[u],t);
		for(auto v:e[u]) if(v!=fa[u]&&v!=son[u]) dfs2(v,v);
	}
	void build(int rt=1){
		root=rt;timer=0;
		fa.assign(n+1,0);dep.assign(n+1,0);sz.assign(n+1,0);son.assign(n+1,0);
		top.assign(n+1,0);dfn.assign(n+1,0);rk.assign(n+1,0);
		dfs1(root,0);dfs2(root,root);
	}
	int lca(int u,int v)const{
		while(top[u]!=top[v]){
			if(dep[top[u]]<dep[top[v]]) swap(u,v);
			u=fa[top[u]];
		}
		return dep[u]<dep[v]?u:v;
	}
	vector<Part> path(int u,int v,bool edge=false)const{
		vector<Part> a,b;
		while(top[u]!=top[v]){
			if(dep[top[u]]>=dep[top[v]]){
				a.push_back({dfn[top[u]],dfn[u],true});u=fa[top[u]];
			}
			else{b.push_back({dfn[top[v]],dfn[v],false});v=fa[top[v]];}
		}
		if(dep[u]>=dep[v]){
			if(dfn[v]+edge<=dfn[u]) a.push_back({dfn[v]+edge,dfn[u],true});
		}
		else if(dfn[u]+edge<=dfn[v]) b.push_back({dfn[u]+edge,dfn[v],false});
		reverse(b.begin(),b.end());a.insert(a.end(),b.begin(),b.end());return a;
	}
	pair<int,int> subtree(int u,bool edge=false)const{return {dfn[u]+edge,dfn[u]+sz[u]-1};}
};
