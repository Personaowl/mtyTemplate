// 依赖 qpow.cpp；模数为素数，2<=mod<=2e9，横坐标模 mod 后两两不同。
long long lagrange(const vector<long long>& xs,const vector<long long>& ys,long long x,long long mod){
	int n=xs.size();long long ans=0;
	auto norm=[&](long long v){v%=mod;if(v<0) v+=mod;return v;};
	x=norm(x);vector<long long> a(n);
	for(int i=0;i<n;i++) a[i]=norm(xs[i]);
	for(int i=0;i<n;i++){
		if(x==a[i]) return norm(ys[i]);
		long long num=1,den=1;
		for(int j=0;j<n;j++) if(i!=j){
			num=num*norm(x-a[j])%mod;
			den=den*norm(a[i]-a[j])%mod;
		}
		ans=(ans+norm(ys[i])*num%mod*qpow(den,mod-2,mod))%mod;
	}
	return ans;
}
// y[i]=P(i)，0<=i<n；次数<n 且 n<=mod，前后缀乘积避开逐项求逆。
long long lagrange_consecutive(const vector<long long>& y,long long x,long long mod){
	int n=y.size();x%=mod;if(x<0) x+=mod;
	if(x<n){long long v=y[x]%mod;return v<0?v+mod:v;}
	vector<long long> pre(n+1,1),suf(n+1,1),fac(n+1,1),inv(n+1,1);
	for(int i=0;i<n;i++) pre[i+1]=pre[i]*(x-i+mod)%mod;
	for(int i=n-1;i>=0;i--) suf[i]=suf[i+1]*(x-i+mod)%mod;
	for(int i=1;i<n;i++) fac[i]=fac[i-1]*i%mod;
	inv[n-1]=qpow(fac[n-1],mod-2,mod);
	for(int i=n-1;i>0;i--) inv[i-1]=inv[i]*i%mod;
	long long ans=0;
	for(int i=0;i<n;i++){
		long long v=y[i]%mod;if(v<0) v+=mod;
		v=v*pre[i]%mod*suf[i+1]%mod*inv[i]%mod*inv[n-1-i]%mod;
		if((n-1-i)&1) v=(mod-v)%mod;
		ans=(ans+v)%mod;
	}
	return ans;
}
