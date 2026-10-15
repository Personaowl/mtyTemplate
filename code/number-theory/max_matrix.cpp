struct MaxMat{
	static constexpr long long INF=-(1LL<<60);
	int n;
	vector<vector<long long>> a;
	MaxMat(int n,bool unit=false):n(n),
		a(n+1,vector<long long>(n+1,INF)){
		if(unit) for(int i=1;i<=n;i++) a[i][i]=0;
	}
	MaxMat operator*(const MaxMat &b) const{
		MaxMat c(n);
		for(int i=1;i<=n;i++){
			for(int k=1;k<=n;k++){
				if(a[i][k]==INF) continue;
				for(int j=1;j<=n;j++){
					if(b.a[k][j]==INF) continue;
					c.a[i][j]=max(c.a[i][j],a[i][k]+b.a[k][j]);
				}
			}
		}
		return c;
	}
	MaxMat pow(long long k) const{
		MaxMat x=*this,res(n,true);
		while(k){
			if(k&1) res=res*x;
			x=x*x;
			k>>=1;
		}
		return res;
	}
};
