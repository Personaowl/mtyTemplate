#define ls (u << 1)
#define rs (u << 1 | 1)

struct MaxSubarraySegTree{
	struct Node{int l,r;long long sum,lmax,rmax,maxn;};
	vector<long long> w;
	vector<Node> tr;
	MaxSubarraySegTree(int n):w(n+1),tr(n*4+1){}
	void pushup(Node& u,const Node& l,const Node& r){
		u.l=l.l;u.r=r.r;u.sum=l.sum+r.sum;
		u.lmax=max(l.lmax,l.sum+r.lmax);
		u.rmax=max(r.rmax,r.sum+l.rmax);
		u.maxn=max({l.maxn,r.maxn,l.rmax+r.lmax});
	}
	void pushup(int u){pushup(tr[u],tr[ls],tr[rs]);}
	void build(int u,int l,int r){
		tr[u]={l,r,0,0,0,0};
		if(l==r){tr[u]={l,r,w[l],w[l],w[l],w[l]};return;}
		int mid=(l+r)>>1;
		build(ls,l,mid);build(rs,mid+1,r);pushup(u);
	}
	void modify(int u,int idx,long long val){
		if(tr[u].l==tr[u].r){
			tr[u]={tr[u].l,tr[u].r,val,val,val,val};return;
		}
		int mid=(tr[u].l+tr[u].r)>>1;
		if(idx<=mid) modify(ls,idx,val);
		else modify(rs,idx,val);
		pushup(u);
	}

	Node query(int u,int L,int R){
		if(L<=tr[u].l&&tr[u].r<=R) return tr[u];

		int mid=(tr[u].l+tr[u].r)>>1;
		if(R<=mid) return query(ls,L,R);
		if(L>mid) return query(rs,L,R);
		Node left=query(ls,L,R),right=query(rs,L,R),res{};
		pushup(res,left,right);return res;
	}
};
#undef ls
#undef rs
