// Number of domino tilings of an n*m board, modulo P.
long long domino_tilings(int n,int m,long long P){
	if(n>m) swap(n,m);
	int S=1LL<<n;vector<vector<int>> to(S);
	for(int mask=0;mask<S;mask++){
		auto dfs=[&](auto&& self,int row,int next)->void{
			if(row==n){to[mask].push_back(next);return;}
			if(mask>>row&1){self(self,row+1,next);return;}
			self(self,row+1,next|(1LL<<row));
			if(row+1<n&&!(mask>>(row+1)&1)) self(self,row+2,next);
		};
		dfs(dfs,0,0);
	}
	vector<long long> f(S);f[0]=1%P;
	for(int col=1;col<=m;col++){
		vector<long long> g(S);
		for(int mask=0;mask<S;mask++) for(int next:to[mask]){
			g[next]=((__int128)g[next]+f[mask])%P;
		}
		f.swap(g);
	}
	return f[0];
}
