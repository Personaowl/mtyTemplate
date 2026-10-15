// Add one 0/1 item with costs w1,w2 and value v.
void two_cost_knapsack(vector<vector<long long>>& f,int w1,int w2,long long v){
	static constexpr long long INF=4e18;
	int V=(int)f.size()-1,U=(int)f[0].size()-1;
	for(int j=V;j>=w1;j--){
		for(int k=U;k>=w2;k--){
			if(f[j-w1][k-w2]==-INF) continue;
			f[j][k]=max(f[j][k],(long long)((__int128)f[j-w1][k-w2]+v));
		}
	}
}
