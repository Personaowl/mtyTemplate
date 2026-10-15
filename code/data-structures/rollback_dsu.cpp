// Based on daily-problems/3-24/U531226; rollback uses union by size only.
struct RollbackDSU{
	vector<int> fa,sz;
	vector<pair<int,int>> st;
	void init(int n){
		fa.resize(n+1);iota(fa.begin(),fa.end(),0);
		sz.assign(n+1,1);st.clear();
	}
	int find(int x)const{
		if(fa[x]==x) return x;
		return find(fa[x]);
	}
	bool check(int x,int y)const{return find(x)==find(y);}
	int size(int x)const{return sz[find(x)];}
	bool merge(int x,int y){
		x=find(x);y=find(y);
		if(x==y) return false;
		if(sz[x]<sz[y]) swap(x,y);
		st.push_back({x,y});fa[y]=x;sz[x]+=sz[y];
		return true;
	}
	int time()const{return st.size();}
	void revert(){
		auto [x,y]=st.back();st.pop_back();
		fa[y]=y;sz[x]-=sz[y];
	}
	void rollback(int t){while(time()>t) revert();}
};
