// Dependency: code/trees/hld.cpp. Following the user's modular P3384 template.
#define ls (u << 1)
#define rs (u << 1 | 1)
struct ModSegTree{
	struct Node{int l,r;long long sum,tag;};
	long long P;
	vector<long long> w;
	vector<Node> tr;
	ModSegTree(int n,long long P):P(P),w(n+1),tr(n*4+1){}
	long long norm(long long x)const{x%=P;if(x<0) x+=P;return x;}
	void eval(Node& u,long long add){
		add=norm(add);
		u.sum=((__int128)u.sum+(__int128)(u.r-u.l+1)*add)%P;
		u.tag=((__int128)u.tag+add)%P;
	}
	void pushdown(Node& u,Node& l,Node& r){
		if(u.tag==0) return;
		eval(l,u.tag);eval(r,u.tag);u.tag=0;
	}
	void pushup(Node& u,const Node& l,const Node& r){
		u.l=l.l;u.r=r.r;u.sum=((__int128)l.sum+r.sum)%P;
	}
	void pushup(int u){pushup(tr[u],tr[ls],tr[rs]);}
	void pushdown(int u){pushdown(tr[u],tr[ls],tr[rs]);}
	void build(int u,int l,int r){
		tr[u]={l,r,0,0};
		if(l==r){tr[u].sum=norm(w[l]);return;}
		int mid=(l+r)>>1;
		build(ls,l,mid);build(rs,mid+1,r);pushup(u);
	}
	void modify(int u,int L,int R,long long val){
		if(L<=tr[u].l&&tr[u].r<=R){eval(tr[u],val);return;}
		pushdown(u);int mid=(tr[u].l+tr[u].r)>>1;
		if(L<=mid) modify(ls,L,R,val);
		if(R>mid) modify(rs,L,R,val);
		pushup(u);
	}
	Node query(int u,int L,int R){
		if(L<=tr[u].l&&tr[u].r<=R) return tr[u];
		pushdown(u);int mid=(tr[u].l+tr[u].r)>>1;
		if(R<=mid) return query(ls,L,R);
		if(L>mid) return query(rs,L,R);
		Node left=query(ls,L,R),right=query(rs,L,R),res{};
		pushup(res,left,right);return res;
	}
};
#undef ls
#undef rs

void path_modify(const HLD& g,ModSegTree& seg,int x,int y,long long z){
	while(g.top[x]!=g.top[y]){
		if(g.dep[g.top[x]]<g.dep[g.top[y]]) swap(x,y);
		seg.modify(1,g.dfn[g.top[x]],g.dfn[x],z);
		x=g.fa[g.top[x]];
	}
	if(g.dep[x]>g.dep[y]) swap(x,y);
	seg.modify(1,g.dfn[x],g.dfn[y],z);
}
long long path_query(const HLD& g,ModSegTree& seg,int x,int y){
	long long res=0;
	while(g.top[x]!=g.top[y]){
		if(g.dep[g.top[x]]<g.dep[g.top[y]]) swap(x,y);
		res=((__int128)res+seg.query(1,g.dfn[g.top[x]],g.dfn[x]).sum)%seg.P;
		x=g.fa[g.top[x]];
	}
	if(g.dep[x]>g.dep[y]) swap(x,y);
	return ((__int128)res+seg.query(1,g.dfn[x],g.dfn[y]).sum)%seg.P;
}
void solve_hld(){
	int n,m,rt;long long P;cin>>n>>m>>rt>>P;
	vector<long long> a(n+1);
	for(int i=1;i<=n;i++) cin>>a[i];
	HLD g;g.init(n);
	for(int i=1;i<n;i++){int u,v;cin>>u>>v;g.add(u,v);}
	g.build(rt);ModSegTree seg(n,P);
	for(int i=1;i<=n;i++) seg.w[g.dfn[i]]=a[i];
	seg.build(1,1,n);
	while(m--){
		int op,x;cin>>op>>x;
		if(op==1){int y;long long z;cin>>y>>z;path_modify(g,seg,x,y,z);}
		else if(op==2){int y;cin>>y;cout<<path_query(g,seg,x,y)<<'\n';}
		else if(op==3){
			long long z;cin>>z;
			seg.modify(1,g.dfn[x],g.dfn[x]+g.sz[x]-1,z);
		}
		else cout<<seg.query(1,g.dfn[x],g.dfn[x]+g.sz[x]-1).sum<<'\n';
	}
}
