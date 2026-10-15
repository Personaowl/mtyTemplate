// Maximum hull: nondecreasing slopes and nondecreasing query x.
struct MonotoneCHT{
	struct Line{long long k,m;__int128 get(long long x)const{return (__int128)k*x+m;}};
	deque<Line> q;
	void init(){q.clear();}
	bool bad(const Line& a,const Line& b,const Line& c)const{
		return ((__int128)a.m-b.m)*((__int128)c.k-b.k)
			>=((__int128)b.m-c.m)*((__int128)b.k-a.k);
	}
	void add_line(long long k,long long m){
		if(!q.empty()&&q.back().k==k){
			if(q.back().m>=m) return;
			q.pop_back();
		}
		Line cur{k,m};
		while(q.size()>=2&&bad(q[q.size()-2],q.back(),cur)) q.pop_back();
		q.push_back(cur);
	}
	__int128 query(long long x){
		while(q.size()>=2&&q[0].get(x)<=q[1].get(x)) q.pop_front();
		return q.front().get(x);
	}
};
vector<long long> quadratic_dp(const vector<long long>& a,long long C){
	int n=(int)a.size()-1;vector<long long> f(n+1);long long s=0;
	MonotoneCHT hull;hull.add_line(0,0);
	for(int i=1;i<=n;i++){
		s+=a[i];f[i]=(__int128)s*s+C-hull.query(s);
		hull.add_line(2*s,-((__int128)f[i]+(__int128)s*s));
	}
	return f;
}
