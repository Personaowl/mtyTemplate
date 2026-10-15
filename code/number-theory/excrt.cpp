// Requires exgcd.cpp.
struct ExCRT{
	long long r=0,m=1;
	void init(){r=0,m=1;}
	int merge(long long b,long long n){
		b%=n;
		if(b<0) b+=n;
		long long x,y,d=exgcd(m,n,x,y),p=n/d;
		__int128 c=(__int128)b-r;
		if(c%d) return 0;
		long long t=(c/d*x)%p;
		if(t<0) t+=p;
		__int128 nm=(__int128)m*p;
		if(nm>LLONG_MAX) return -1;
		r=((__int128)r+(__int128)m*t)%nm;
		m=nm;
		return 1;
	}
};
