// Requires exgcd.cpp.
long long inv_mod(long long a,long long mod){
	a%=mod;
	if(a<0) a+=mod;
	long long x,y,d=exgcd(a,mod,x,y);
	if(d!=1) return -1;
	x%=mod;
	if(x<0) x+=mod;
	return x;
}
vector<long long> inverse_table(int n,long long p){
	vector<long long> inv(n+1);
	if(n>=1) inv[1]=1;
	for(int i=2;i<=n;i++) inv[i]=(p-p/i)*inv[p%i]%p;
	return inv;
}
