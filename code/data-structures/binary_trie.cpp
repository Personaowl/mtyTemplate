struct BinaryTrie{
	using ull=uint64_t;
	int B;
	vector<array<int32_t,2>> tr;
	vector<int> cnt;
	BinaryTrie(int bits=63){init(bits);}
	void init(int bits=63){
		B=bits;tr.assign(1,{});cnt.assign(1,0);
	}
	int size()const{return cnt[0];}
	void insert(ull x){
		int u=0;cnt[u]++;
		for(int i=B-1;i>=0;i--){
			int b=x>>i&1;
			if(!tr[u][b]){
				int v=tr.size();tr[u][b]=v;
				tr.push_back({});cnt.push_back(0);
			}
			u=tr[u][b];cnt[u]++;
		}
	}
	int count(ull x)const{
		int u=0;
		for(int i=B-1;i>=0;i--){
			u=tr[u][x>>i&1];
			if(!u||!cnt[u]) return 0;
		}
		return cnt[u];
	}
	bool erase(ull x){
		if(!count(x)) return false;
		int u=0;cnt[u]--;
		for(int i=B-1;i>=0;i--){u=tr[u][x>>i&1];cnt[u]--;}
		return true;
	}
	pair<bool,ull> query(ull x,bool maxn=true)const{
		if(!cnt[0]) return {false,0};
		int u=0;ull res=0;
		for(int i=B-1;i>=0;i--){
			int b=x>>i&1,v=b^maxn;
			if(!tr[u][v]||!cnt[tr[u][v]]) v^=1;
			if(v!=b) res|=1ULL<<i;
			u=tr[u][v];
		}
		return {true,res};
	}
};
