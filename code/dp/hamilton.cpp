// Vertex numbers are 0..n-1, matching the bit positions.
long long hamilton_path(const vector<vector<long long>>& w,int start,int target){
	static constexpr long long INF=4e18;
	int n=w.size(),S=1LL<<n;
	vector<vector<long long>> f(S,vector<long long>(n,INF));
	f[1LL<<start][start]=0;
	for(int mask=0;mask<S;mask++) for(int u=0;u<n;u++){
		if(f[mask][u]==INF) continue;
		for(int v=0;v<n;v++) if(!(mask>>v&1)&&w[u][v]!=INF){
			int next=mask|(1LL<<v);
			f[next][v]=min(f[next][v],(long long)((__int128)f[mask][u]+w[u][v]));
		}
	}
	return f[S-1][target];
}
