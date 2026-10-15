// Requires HLD. Example statistic: distinct colors in each rooted subtree.
struct DSUOnTree{
	HLD tr;
	int m,types;
	vector<int> color,cnt,ans;
	void init(int n,int m){tr.init(n);this->m=m;}
	void add(int u,int v){tr.add(u,v);}
	void eval(int u,int d){
		int c=color[u];if(cnt[c]) types--;
		cnt[c]+=d;if(cnt[c]) types++;
	}
	void subtree(int u,int d){
		for(int i=tr.dfn[u];i<tr.dfn[u]+tr.sz[u];i++) eval(tr.rk[i],d);
	}
	void dfs(int u,bool keep){
		for(auto v:tr.e[u]) if(v!=tr.fa[u]&&v!=tr.son[u]) dfs(v,false);
		if(tr.son[u]) dfs(tr.son[u],true);
		for(auto v:tr.e[u]) if(v!=tr.fa[u]&&v!=tr.son[u]) subtree(v,1);
		eval(u,1);ans[u]=types;
		if(!keep) subtree(u,-1);
	}
	vector<int> run(const vector<int> &a,int root=1){
		tr.build(root);color=a;cnt.assign(m+1,0);ans.assign(tr.n+1,0);types=0;
		dfs(root,true);return ans;
	}
};
