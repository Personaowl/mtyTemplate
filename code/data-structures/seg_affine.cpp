#define ls (u << 1)
#define rs (u << 1 | 1)

struct AffineSegTree{
	struct Node{int l,r;long long sum,mul,add;};
	long long P;
	vector<long long> w;
	vector<Node> tr;
	AffineSegTree(int n,long long P):P(P),w(n+1),tr(n*4+1){}
	long long norm(long long x)const{x%=P;if(x<0) x+=P;return x;}
	void eval(Node& u,long long mul,long long add){
		u.sum=((__int128)u.sum*mul+(__int128)(u.r-u.l+1)*add)%P;
		u.mul=(__int128)u.mul*mul%P;
		u.add=((__int128)u.add*mul+add)%P;
	}
	void pushdown(Node& u,Node& l,Node& r){
		if(u.mul==1%P&&u.add==0) return;
		eval(l,u.mul,u.add);eval(r,u.mul,u.add);
		u.mul=1%P;u.add=0;
	}
	void pushup(Node& u,const Node& l,const Node& r){
		u.l=l.l;u.r=r.r;u.sum=((__int128)l.sum+r.sum)%P;
	}
	void pushup(int u){pushup(tr[u],tr[ls],tr[rs]);}
	void pushdown(int u){pushdown(tr[u],tr[ls],tr[rs]);}
	void build(int u,int l,int r){
		tr[u]={l,r,0,1%P,0};
		if(l==r){tr[u].sum=norm(w[l]);return;}
		int mid=(l+r)>>1;
		build(ls,l,mid);build(rs,mid+1,r);pushup(u);
	}
	void modify(int u,int L,int R,long long mul,long long add){
		mul=norm(mul);add=norm(add);
		if(L<=tr[u].l&&tr[u].r<=R){eval(tr[u],mul,add);return;}
		pushdown(u);
		int mid=(tr[u].l+tr[u].r)>>1;
		if(L<=mid) modify(ls,L,R,mul,add);
		if(R>mid) modify(rs,L,R,mul,add);
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
