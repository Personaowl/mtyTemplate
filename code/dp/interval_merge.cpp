// Merge adjacent piles; circular=true permits choosing the cut of a ring.
long long merge_stones(const vector<long long>& a,bool circular=false){
	static constexpr long long INF=4e18;
	int n=(int)a.size()-1,m=circular?n*2:n;
	vector<long long> s(m+1);
	for(int i=1;i<=m;i++) s[i]=s[i-1]+a[(i-1)%n+1];
	vector<vector<long long>> f(m+2,vector<long long>(m+2));
	for(int len=2;len<=n;len++) for(int l=1;l+len-1<=m;l++){
		int r=l+len-1;f[l][r]=INF;
		for(int k=l;k<r;k++){
			long long val=(__int128)f[l][k]+f[k+1][r]+s[r]-s[l-1];
			f[l][r]=min(f[l][r],val);
		}
	}
	long long ans=f[1][n];
	if(circular) for(int l=2;l<=n;l++) ans=min(ans,f[l][l+n-1]);
	return ans;
}
