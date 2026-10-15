__int128 remainder_sum(long long n,long long k){
	__int128 res=(__int128)n*k;
	for(long long l=1,r;l<=min(n,k);l=r+1){
		long long v=k/l;
		r=min(n,k/v);
		res-=(__int128)v*((__int128)l+r)*(r-l+1)/2;
	}
	return res;
}
