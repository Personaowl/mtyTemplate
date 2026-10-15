// Offline distinct values; compressed values are 1..m.
struct Mo{
	struct Query{int l,r,id;};
	int n,m,len;
	vector<int> a;
	vector<Query> q;
	void init(const vector<int>& w,int m){
		a=w;n=(int)a.size()-1;this->m=m;q.clear();
	}
	int add_query(int l,int r){
		int id=q.size();q.push_back({l,r,id});return id;
	}
	vector<int> work(){
		int Q=q.size();if(!Q) return {};
		len=max((int)1,(int)(n/sqrt(Q)));
		auto jobs=q;
		sort(jobs.begin(),jobs.end(),[&](const Query& x,const Query& y){
			int bx=(x.l-1)/len,by=(y.l-1)/len;
			if(bx!=by) return bx<by;
			return bx&1?x.r>y.r:x.r<y.r;
		});
		vector<int> cnt(m+1),ans(Q);int l=1,r=0,now=0;
		auto add=[&](int p){if(cnt[a[p]]++==0) now++;};
		auto del=[&](int p){if(--cnt[a[p]]==0) now--;};
		for(auto [L,R,id]:jobs){
			while(l>L) add(--l);
			while(r<R) add(++r);
			while(l<L) del(l++);
			while(r>R) del(r--);
			ans[id]=now;
		}
		return ans;
	}
};
