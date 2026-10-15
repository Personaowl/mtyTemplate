long long exgcd(long long a,long long b,long long &x,long long &y){
	if(!b){
		x=a>=0?1:-1,y=0;
		return a>=0?a:-a;
	}
	long long x1,y1,d=exgcd(b,a%b,x1,y1);
	x=y1;
	y=(__int128)x1-(__int128)(a/b)*y1;
	return d;
}
