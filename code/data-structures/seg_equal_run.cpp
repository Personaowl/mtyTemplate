#define ls (u << 1)
#define rs (u << 1 | 1)

// Range assignment and longest contiguous run of equal values.
struct EqualRunSegTree{
	struct Node{
		int l,r,lsum,rsum,maxn;
		long long lc,rc,tag;
		bool has;
	};
	vector<long long> w;
	vector<Node> tr;
	EqualRunSegTree(int n):w(n+1),tr(n*4+1){}
	void eval(Node& u,long long val){
		u.lc=u.rc=u.tag=val;u.has=true;
		u.lsum=u.rsum=u.maxn=u.r-u.l+1;
	}
	void pushdown(Node& u,Node& l,Node& r){
		if(!u.has) return;
		eval(l,u.tag);eval(r,u.tag);u.has=false;
	}
	void pushup(Node& u,const Node& l,const Node& r){
		u.l=l.l;u.r=r.r;u.lc=l.lc;u.rc=r.rc;
		u.lsum=l.lsum;u.rsum=r.rsum;u.maxn=max(l.maxn,r.maxn);
		if(l.rc==r.lc){
			u.maxn=max(u.maxn,l.rsum+r.lsum);
			if(l.lsum==l.r-l.l+1) u.lsum+=r.lsum;
			if(r.rsum==r.r-r.l+1) u.rsum+=l.rsum;
		}
	}
	void pushup(int u){pushup(tr[u],tr[ls],tr[rs]);}
	void pushdown(int u){pushdown(tr[u],tr[ls],tr[rs]);}
	void build(int u,int l,int r){
		tr[u]={};tr[u].l=l;tr[u].r=r;
		if(l==r){eval(tr[u],w[l]);return;}
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
