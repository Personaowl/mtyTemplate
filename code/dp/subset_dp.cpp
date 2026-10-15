// f[mask] becomes the sum of a[sub] over all submasks sub of mask.
vector<long long> subset_sums(vector<long long> f,int n){
	int S=1LL<<n;
	for(int i=0;i<n;i++) for(int mask=0;mask<S;mask++){
		if(mask>>i&1) f[mask]+=f[mask^(1LL<<i)];
	}
	return f;
}
