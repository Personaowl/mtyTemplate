struct Mat{
	int n;
	long long mod;
	vector<vector<long long>> a;
	Mat(int n,long long mod,bool unit=false):n(n),mod(mod),
		a(n+1,vector<long long>(n+1,0)){
		if(unit) for(int i=1;i<=n;i++) a[i][i]=1%mod;
	}
	Mat operator*(const Mat &b) const{
		Mat c(n,mod);
		for(int i=1;i<=n;i++){
			for(int k=1;k<=n;k++){
				if(!a[i][k]) continue;
				for(int j=1;j<=n;j++){
					c.a[i][j]=(c.a[i][j]+a[i][k]*b.a[k][j])%mod;
				}
			}
		}
		return c;
	}
	Mat pow(long long k) const{
		Mat x=*this,res(n,mod,true);
		while(k){
			if(k&1) res=res*x;
			x=x*x;
			k>>=1;
		}
		return res;
	}
};
