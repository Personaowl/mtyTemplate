struct MoWithUpdate{
	struct Query{int l,r,t,id;};
	struct Update{int p,val;};
	int n,m;
	vector<int> a;
	vector<Query> q;
	vector<Update> upd;
	void init(const vector<int>& w,int m){
		a=w;n=(int)a.size()-1;this->m=m;q.clear();upd.clear();
	}
	void modify(int p,int val){upd.push_back({p,val});}
	int add_query(int l,int r){
		int id=q.size();q.push_back({l,r,(int)upd.size(),id});return id;
	}
	vector<int> work()const{
		int len=max((int)1,(int)pow(n,2.0/3));
		auto jobs=q;auto changes=upd;auto w=a;
		sort(jobs.begin(),jobs.end(),[&](const Query& x,const Query& y){
			int xl=(x.l-1)/len,yl=(y.l-1)/len;
			if(xl!=yl) return xl<yl;
			int xr=(x.r-1)/len,yr=(y.r-1)/len;
			if(xr!=yr) return xr<yr;
			return x.t<y.t;
		});
		vector<int> cnt(m+1),ans(q.size());int l=1,r=0,t=0,now=0;
		auto add=[&](int val){if(cnt[val]++==0) now++;};
		auto del=[&](int val){if(--cnt[val]==0) now--;};
		auto move=[&](int i){
			auto [p,val]=changes[i];
			if(l<=p&&p<=r){del(w[p]);add(val);}
			swap(w[p],changes[i].val);
		};
		for(auto [L,R,T,id]:jobs){
			while(l>L) add(w[--l]);
			while(r<R) add(w[++r]);
			while(l<L) del(w[l++]);
			while(r>R) del(w[r--]);
			while(t<T) move(t++);
			while(t>T) move(--t);
			ans[id]=now;
		}
		return ans;
	}
};
