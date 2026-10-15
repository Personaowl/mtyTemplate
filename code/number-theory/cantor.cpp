long long cantor_rank(const vector<int> &a,long long mod){
	int n=(int)a.size()-1;
	vector<long long> fac(n+1,1);
	vector<int> tr(n+1,0);
	for(int i=1;i<=n;i++){
		fac[i]=fac[i-1]*i%mod;
		tr[i]=i&-i;
	}
	auto sum=[&](int x){
		int res=0;
		for(;x;x-=x&-x) res+=tr[x];
		return res;
	};
	long long res=0;
	for(int i=1;i<=n;i++){
		res=(res+sum(a[i]-1)*fac[n-i])%mod;
		for(int x=a[i];x<=n;x+=x&-x) tr[x]--;
	}
	return (res+1)%mod;
}
