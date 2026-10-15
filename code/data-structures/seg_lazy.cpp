#define ls (u << 1)
#define rs (u << 1 | 1)

// Following the user's P3384 segment tree; non-modular range add and sum.
struct SegTree{
	struct Node{int l,r;long long sum,tag;};
	vector<long long> w;
	vector<Node> tr;
	SegTree(int n):w(n+1),tr(n*4+1){}
	void eval(Node& u,long long add){
		u.sum+=(u.r-u.l+1)*add;u.tag+=add;
	}
	void pushdown(Node& u,Node& l,Node& r){
		if(u.tag==0) return;
		eval(l,u.tag);eval(r,u.tag);u.tag=0;
	}
	void pushup(Node& u,const Node& l,const Node& r){
		u.l=l.l;u.r=r.r;u.sum=l.sum+r.sum;
	}
	void pushup(int u){pushup(tr[u],tr[ls],tr[rs]);}
	void pushdown(int u){pushdown(tr[u],tr[ls],tr[rs]);}
	void build(int u,int l,int r){
		tr[u]={l,r,0,0};
		if(l==r){tr[u].sum=w[l];return;}
		int mid=(l+r)>>1;
		build(ls,l,mid);build(rs,mid+1,r);pushup(u);
	}
	void modify(int u,int L,int R,long long val){
		if(L<=tr[u].l&&tr[u].r<=R){eval(tr[u],val);return;}
		pushdown(u);
		int mid=(tr[u].l+tr[u].r)>>1;
		if(L<=mid) modify(ls,L,R,val);
		if(R>mid) modify(rs,L,R,val);
		pushup(u);
	}

	Node query(int u,int L,int R){
		if(L<=tr[u].l&&tr[u].r<=R) return tr[u];
		pushdown(u);
		int mid=(tr[u].l+tr[u].r)>>1;
		if(R<=mid) return query(ls,L,R);
		if(L>mid) return query(rs,L,R);
		Node left=query(ls,L,R),right=query(rs,L,R),res{};
		pushup(res,left,right);return res;
	}
};
#undef ls
#undef rs
