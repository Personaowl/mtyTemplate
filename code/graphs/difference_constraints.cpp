// Requires spfa.cpp.
struct DifferenceConstraints{
	SPFA sp;
	vector<long long> val;
	void init(int n){sp.init(n);val.clear();}
	void le(int u,int v,long long w){sp.add(v,u,w);}
	void ge(int u,int v,long long w){le(v,u,-w);}
	void eq(int u,int v,long long w){le(u,v,w);le(v,u,-w);}
	bool solve(){
		val.clear();
		if(!sp.run()) return false;
		val=sp.dis;
		return true;
	}
};
