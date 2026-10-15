// Dependency: dsu.cpp. x and x+n represent opposite kinds.
struct BinaryDSU{
	int n;
	DSU dsu;
	void init(int n){this->n=n;dsu.init(n*2);}
	bool same(int x,int y){return dsu.check(x,y);}
	bool different(int x,int y){return dsu.check(x,y+n);}
	bool merge(int x,int y,int r){
		if(r==0){
			if(different(x,y)) return false;
			dsu.merge(x,y);dsu.merge(x+n,y+n);
		}
		else{
			if(same(x,y)) return false;
			dsu.merge(x,y+n);dsu.merge(x+n,y);
		}
		return true;
	}
	pair<bool,int> query(int x,int y){
		if(same(x,y)) return {true,0};
		if(different(x,y)) return {true,1};
		return {false,0};
	}
};
