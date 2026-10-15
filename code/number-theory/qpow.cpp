long long qpow(long long a,long long b,long long mod){
	a%=mod;
	if(a<0) a+=mod;
	long long res=1%mod;
	while(b){
		if(b&1) res=res*a%mod;
		a=a*a%mod;
		b>>=1;
	}
	return res;
}
