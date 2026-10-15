// Prefix frequency versions; node 0 is the shared empty tree.
struct PersistentSegTree{
	struct Node{int ls=0,rs=0,sum=0;};
	int n,m;
	vector<Node> tr;
	vector<int> root;
	vector<long long> b;
	int change(int pre,int l,int r,int p){
		Node old=tr[pre];int u=tr.size();tr.push_back(old);tr[u].sum++;
		if(l==r) return u;
		int mid=(l+r)>>1;
		if(p<=mid){int v=change(tr[u].ls,l,mid,p);tr[u].ls=v;}
		else{int v=change(tr[u].rs,mid+1,r,p);tr[u].rs=v;}
		return u;
	}
	void init(const vector<long long>& a){
		n=(int)a.size()-1;b.assign(a.begin()+1,a.end());
		sort(b.begin(),b.end());b.erase(unique(b.begin(),b.end()),b.end());
		m=b.size();tr.assign(1,{});root.assign(n+1,0);
		for(int i=1;i<=n;i++){
			int p=lower_bound(b.begin(),b.end(),a[i])-b.begin()+1;
			root[i]=change(root[i-1],1,m,p);
		}
	}
	int kth(int u,int v,int l,int r,int k)const{
		if(l==r) return l;
		int mid=(l+r)>>1,cnt=tr[tr[v].ls].sum-tr[tr[u].ls].sum;
		if(k<=cnt) return kth(tr[u].ls,tr[v].ls,l,mid,k);
		return kth(tr[u].rs,tr[v].rs,mid+1,r,k-cnt);
	}
	long long query(int l,int r,int k)const{
		return b[kth(root[l-1],root[r],1,m,k)-1];
	}
};
