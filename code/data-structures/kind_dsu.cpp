// Dependency: dsu.cpp. Expanded node id(x,t) represents class[x]+t modulo k.
struct KindDSU{
	int n,k;
	DSU dsu;
	void init(int n,int k=2){this->n=n;this->k=k;dsu.init(n*k);}
	int id(int x,int t)const{return x+t*n;}
	pair<bool,int> query(int x,int y){
		for(int t=0;t<k;t++){
			if(dsu.check(id(x,t),id(y,0))) return {true,t};
		}
		return {false,0};
	}
	bool check(int x,int y){return query(x,y).first;}
	bool merge(int x,int y,int r){
		r%=k;if(r<0) r+=k;
		auto [known,val]=query(x,y);
		if(known) return val==r;
		for(int t=0;t<k;t++) dsu.merge(id(x,(t+r)%k),id(y,t));
		return true;
	}
};
