struct DynamicSegTree{
	struct Node{int ls=0,rs=0;long long sum=0,tag=0;bool has=false;};
	long long L,R;
	int root;
	vector<Node> tr;
	void init(long long l,long long r){L=l;R=r;root=0;tr.assign(1,{});}
	int node(){tr.push_back({});return (int)tr.size()-1;}
	void eval(int u,long long l,long long r,long long val){
		tr[u].sum=(r-l+1)*val;tr[u].tag=val;tr[u].has=true;
	}
	void pushdown(int u,long long l,long long r){
		if(!tr[u].has) return;
		if(!tr[u].ls){int v=node();tr[u].ls=v;}
		if(!tr[u].rs){int v=node();tr[u].rs=v;}
		long long mid=l+(r-l)/2;
		eval(tr[u].ls,l,mid,tr[u].tag);
		eval(tr[u].rs,mid+1,r,tr[u].tag);tr[u].has=false;
	}
	int change(int u,long long l,long long r,
			long long x,long long y,long long val){
		if(!u) u=node();
		if(x<=l&&r<=y){eval(u,l,r,val);return u;}
		pushdown(u,l,r);
		long long mid=l+(r-l)/2;
		if(x<=mid){int v=change(tr[u].ls,l,mid,x,y,val);tr[u].ls=v;}
		if(y>mid){int v=change(tr[u].rs,mid+1,r,x,y,val);tr[u].rs=v;}
		tr[u].sum=tr[tr[u].ls].sum+tr[tr[u].rs].sum;return u;
	}
	long long query(int u,long long l,long long r,
			long long x,long long y)const{
		if(!u) return 0;
		if(x<=l&&r<=y) return tr[u].sum;
		if(tr[u].has){
			return (min(r,y)-max(l,x)+1)*tr[u].tag;
		}
		long long mid=l+(r-l)/2,res=0;
		if(x<=mid) res+=query(tr[u].ls,l,mid,x,y);
		if(y>mid) res+=query(tr[u].rs,mid+1,r,x,y);
		return res;
	}
	void modify(long long l,long long r,long long val){
		root=change(root,L,R,l,r,val);
	}
	long long query(long long l,long long r)const{
		return query(root,L,R,l,r);
	}
};
