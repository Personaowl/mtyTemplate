// Based on user supplied code and KACTL LineContainer (Simon Lindholm, CC0).
// https://github.com/kth-competitive-programming/kactl/blob/main/content/data-structures/LineContainer.h
struct HullLine{
	long long k,m;
	mutable __int128 p;
	bool operator<(const HullLine& o)const{return k<o.k;}
	bool operator<(long long x)const{return p<x;}
};
struct LineContainer:multiset<HullLine,less<>>{
	static constexpr __int128 INF=(__int128)1<<126;
	__int128 div_floor(__int128 a,__int128 b)const{
		__int128 q=a/b,r=a%b;
		if(r!=0&&((r>0)!=(b>0))) q--;
		return q;
	}
	bool isect(iterator x,iterator y){
		if(y==end()){x->p=INF;return false;}
		if(x->k==y->k) x->p=x->m>y->m?INF:-INF;
		else x->p=div_floor((__int128)y->m-x->m,(__int128)x->k-y->k);
		return x->p>=y->p;
	}
	void add_line(long long k,long long m){
		auto z=insert({k,m,0}),y=z++,x=y;
		while(isect(y,z)) z=erase(z);
		if(x!=begin()&&isect(--x,y)) isect(x,y=erase(y));
		while((y=x)!=begin()&&(--x)->p>=y->p) isect(x,erase(y));
	}
	__int128 query(long long x)const{
		auto l=*lower_bound(x);return (__int128)l.k*x+l.m;
	}
};
vector<long long> quadratic_dp_dynamic(const vector<long long>& a,long long C){
	int n=(int)a.size()-1;vector<long long> f(n+1);long long s=0;
	LineContainer hull;hull.add_line(0,0);
	for(int i=1;i<=n;i++){
		s+=a[i];f[i]=(__int128)s*s+C-hull.query(s);
		hull.add_line(2*s,-((__int128)f[i]+(__int128)s*s));
	}
	return f;
}
