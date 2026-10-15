// Requires TreeLCA. Binary jump once, then index a long-chain ladder.
struct LongChain{
	TreeLCA tr;
	vector<int> len,son,top,lg;
	vector<vector<int>> up,down;
	void init(int n){tr.init(n);}
	void add(int u,int v){tr.add(u,v);}
	void build(int root=1){
		tr.build(root);int n=tr.n;
		len.assign(n+1,0);son.assign(n+1,0);top.assign(n+1,0);
		up.assign(n+1,{});down.assign(n+1,{});lg.assign(n+1,0);
		for(int i=2;i<=n;i++) lg[i]=lg[i/2]+1;
		auto dfs1=[&](auto &&self,int u,int p)->void{
			len[u]=1;
			for(auto [v,w]:tr.e[u]) if(v!=p){
				self(self,v,u);if(len[v]>len[son[u]]) son[u]=v;
			}
			if(son[u]) len[u]=len[son[u]]+1;
		};
		auto dfs2=[&](auto &&self,int u,int t)->void{
			top[u]=t;if(son[u]) self(self,son[u],t);
			for(auto [v,w]:tr.e[u]) if(v!=tr.fa[u][0]&&v!=son[u]) self(self,v,v);
		};
		dfs1(dfs1,root,0);dfs2(dfs2,root,root);
		for(int t=1;t<=n;t++) if(top[t]==t){
			int u=t,v=t;
			for(int i=0;i<len[t];i++){
				down[t].push_back(u);up[t].push_back(v);
				u=son[u];v=tr.fa[v][0];
			}
		}
	}
	int jump(int u,long long k)const{
		if(k<0||k>=tr.dep[u]) return 0;
		if(!k) return u;
		int j=lg[k];u=tr.fa[u][j];k-=1LL<<j;
		int t=top[u],d=tr.dep[u]-tr.dep[t]-k;
		return d>=0?down[t][d]:up[t][-d];
	}
};
