// d[x] = value[x] - value[fa[x]]; merge(x,y,w) enforces value[y]-value[x]=w.
struct WeightedDSU{
	vector<int> fa,sz;
	vector<long long> d;
	void init(int n){
		fa.resize(n+1);iota(fa.begin(),fa.end(),0);
		sz.assign(n+1,1);d.assign(n+1,0);
	}
	int find(int x){
		if(fa[x]==x) return x;
		int p=fa[x];fa[x]=find(p);
		d[x]=(__int128)d[x]+d[p];return fa[x];
	}
	bool check(int x,int y){return find(x)==find(y);}
	int size(int x){return sz[find(x)];}
	pair<bool,long long> query(int x,int y){
		if(find(x)!=find(y)) return {false,0};
		return {true,(__int128)d[y]-d[x]};
	}
	bool merge(int x,int y,long long w){
		int px=find(x),py=find(y);
		if(px==py) return (__int128)d[y]-d[x]==w;
		__int128 val=(__int128)w+d[x]-d[y];
		if(sz[px]<sz[py]){swap(px,py);val=-val;}
		fa[py]=px;d[py]=val;sz[px]+=sz[py];return true;
	}
};
