// Integer lines, minimum queries; optionally restrict a line to [ql,qr].
struct LiChaoTree{
	static constexpr long long INF=4e18;
	struct Line{
		long long k=0,b=0;
		__int128 get(long long x)const{return (__int128)k*x+b;}
	};
	struct Node{int ls=0,rs=0;Line f;};
	long long L,R;
	int root;
	vector<Node> tr;
	void init(long long l,long long r){L=l;R=r;root=0;tr.assign(1,{});}
	int node(Line f){
		tr.push_back({0,0,f});return (int)tr.size()-1;
	}
	int insert(int u,long long l,long long r,Line f){
		if(!u) return node(f);
		long long mid=l+(r-l)/2;
		if(f.get(mid)<tr[u].f.get(mid)) swap(f,tr[u].f);
		if(l==r) return u;
		if(f.get(l)<tr[u].f.get(l)){
			int v=insert(tr[u].ls,l,mid,f);tr[u].ls=v;
		}
		else if(f.get(r)<tr[u].f.get(r)){
			int v=insert(tr[u].rs,mid+1,r,f);tr[u].rs=v;
		}
		return u;
	}
	int change(int u,long long l,long long r,
			long long ql,long long qr,Line f){
		if(ql<=l&&r<=qr) return insert(u,l,r,f);
		if(!u) u=node({0,INF});
		long long mid=l+(r-l)/2;
		if(ql<=mid){int v=change(tr[u].ls,l,mid,ql,qr,f);tr[u].ls=v;}
		if(qr>mid){int v=change(tr[u].rs,mid+1,r,ql,qr,f);tr[u].rs=v;}
		return u;
	}
	long long query(int u,long long l,long long r,long long x)const{
		if(!u) return INF;
		long long res=tr[u].f.get(x);
		if(l==r) return res;
		long long mid=l+(r-l)/2;
		if(x<=mid) return min(res,query(tr[u].ls,l,mid,x));
		return min(res,query(tr[u].rs,mid+1,r,x));
	}
	void add(long long k,long long b){root=insert(root,L,R,{k,b});}
	void add(long long l,long long r,long long k,long long b){
		root=change(root,L,R,l,r,{k,b});
	}
	long long query(long long x)const{return query(root,L,R,x);}
};
