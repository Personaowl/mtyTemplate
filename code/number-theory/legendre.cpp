long long vp_fact(long long n,long long p){
	long long res=0;
	while(n){
		n/=p;
		res+=n;
	}
	return res;
}
