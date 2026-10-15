struct PersistentArray{
	struct Node{int ls=0,rs=0;long long val=0;};
	int n;
	vector<Node> tr;
	vector<int> root;
	int build(int l,int r,const vector<long long>& a){
		int u=tr.size();tr.push_back({});
		if(l==r){tr[u].val=a[l];return u;}
		int mid=(l+r)>>1;
		int x=build(l,mid,a),y=build(mid+1,r,a);
		tr[u].ls=x;tr[u].rs=y;return u;
	}
	void init(const vector<long long>& a){
		n=(int)a.size()-1;tr.assign(1,{});root.clear();
		root.push_back(build(1,n,a));
	}
	int change(int pre,int l,int r,int p,long long val){
		Node old=tr[pre];int u=tr.size();tr.push_back(old);
		if(l==r){tr[u].val=val;return u;}
		int mid=(l+r)>>1;
		if(p<=mid){int v=change(tr[u].ls,l,mid,p,val);tr[u].ls=v;}
		else{int v=change(tr[u].rs,mid+1,r,p,val);tr[u].rs=v;}
		return u;
	}
	int modify(int ver,int p,long long val){
		int u=change(root[ver],1,n,p,val);root.push_back(u);
		return (int)root.size()-1;
	}
	int copy(int ver){root.push_back(root[ver]);return (int)root.size()-1;}
	long long query(int u,int l,int r,int p)const{
		if(l==r) return tr[u].val;
		int mid=(l+r)>>1;
		if(p<=mid) return query(tr[u].ls,l,mid,p);
		return query(tr[u].rs,mid+1,r,p);
	}
	long long query(int ver,int p)const{return query(root[ver],1,n,p);}
};
