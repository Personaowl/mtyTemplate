// Requires qpow.cpp.
long long phi(long long n){
	long long res=n;
	for(long long p=2;p<=n/p;p++){
		if(n%p) continue;
		res=res/p*(p-1);
		while(n%p==0) n/=p;
	}
	if(n>1) res=res/n*(n-1);
	return res;
}
long long pow_decimal(long long a,const string &b,long long mod){
	long long t=phi(mod),rem=0,cap=0;
	for(auto ch:b){
		int d=ch-'0';
		rem=(rem*10+d)%t;
		cap=min(t,cap*10+d);
	}
	long long e=rem+(cap==t?t:0);
	return qpow(a,e,mod);
}
