// Dependency: rollback_dsu.cpp. Based on the user's time-segment bipartite template.
#define ls (u << 1)
#define rs (u << 1 | 1)
struct TimeSegTree{
	struct Node{
		int l,r;
		vector<pair<int,int>> E;
	};
	int n;
	vector<int> ans;
	vector<Node> tr;
	TimeSegTree(int k,int n):n(n),ans(k+1,0),tr(k*4+1){}
	void build(int u,int l,int r){
		tr[u]={l,r,{}};
		if(l==r) return;
		int mid=(l+r)>>1;
		build(ls,l,mid);build(rs,mid+1,r);
	}
	void modify(int u,int L,int R,pair<int,int> e){
		if(L>R) return;
		if(L<=tr[u].l&&tr[u].r<=R){
			tr[u].E.push_back(e);return;
		}
		int mid=(tr[u].l+tr[u].r)>>1;
		if(L<=mid) modify(ls,L,R,e);
		if(R>mid) modify(rs,L,R,e);
	}
	void dfs(int u,RollbackDSU& dsu){
		bool flag=true;int history=dsu.time();
		for(auto [x,y]:tr[u].E){
			if(dsu.find(x)==dsu.find(y)){
				flag=false;break;
			}
			dsu.merge(x,y+n);dsu.merge(x+n,y);
		}
		if(!flag){
			for(int i=tr[u].l;i<=tr[u].r;i++) ans[i]=0;
		}
		else if(tr[u].l==tr[u].r) ans[tr[u].l]=1;
		else{dfs(ls,dsu);dfs(rs,dsu);}
		dsu.rollback(history);
	}
};
#undef ls
#undef rs

void solve_time_bipartite(){
	int n,m,k;cin>>n>>m>>k;
	RollbackDSU dsu;dsu.init(2*n);
	TimeSegTree seg(k,n);seg.build(1,1,k);
	for(int i=1;i<=m;i++){
		int u,v,l,r;cin>>u>>v>>l>>r;
		if(l<r) seg.modify(1,l+1,r,{u,v});
	}
	seg.dfs(1,dsu);
	for(int t=1;t<=k;t++) cout<<(seg.ans[t]?"Yes":"No")<<'\n';
}
