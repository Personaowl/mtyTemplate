// At-most capacity 0/1 knapsack; ties prefer not taking the current item.
pair<long long,vector<int>> knapsack_trace(const vector<int>& w,
		const vector<long long>& v,int V){
	int n=(int)w.size()-1;
	vector<vector<long long>> f(n+1,vector<long long>(V+1));
	for(int i=1;i<=n;i++) for(int j=0;j<=V;j++){
		f[i][j]=f[i-1][j];
		if(j>=w[i]) f[i][j]=max(f[i][j],(long long)((__int128)f[i-1][j-w[i]]+v[i]));
	}
	vector<int> chosen;int j=V;
	for(int i=n;i>=1;i--) if(f[i][j]>f[i-1][j]){
		chosen.push_back(i);j-=w[i];
	}
	reverse(chosen.begin(),chosen.end());return {f[n][V],chosen};
}
