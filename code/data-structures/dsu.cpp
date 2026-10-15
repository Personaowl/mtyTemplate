struct DSU{
	vector<int> fa,sz;
	void init(int n){
		fa.resize(n+1);
		sz.assign(n+1,1);
		iota(fa.begin(),fa.end(),0);
	}
	int find(int x){
		if(fa[x]==x) return x;
		return fa[x]=find(fa[x]);
	}
	bool merge(int x,int y){
		x=find(x);
		y=find(y);
		if(x==y) return false;
		if(sz[x]<sz[y]) swap(x,y);
		fa[y]=x;
		sz[x]+=sz[y];
		return true;
	}
	bool check(int x,int y){
		return find(x)==find(y);
	}
	int size(int x){
		return sz[find(x)];
	}
};
