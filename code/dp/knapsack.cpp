// f[j]: best value with capacity <= j, or with exact cost j.
// Initialize f to 0 for <=; for exact cost use -INF except f[0]=0.
void zero_one(vector<long long>& f,int w,long long v){
	static constexpr long long INF=4e18;
	int V=(int)f.size()-1;
	for(int j=V;j>=w;j--){
		if(f[j-w]==-INF) continue;
		f[j]=max(f[j],(long long)((__int128)f[j-w]+v));
	}
}
void complete_knapsack(vector<long long>& f,int w,long long v){
	static constexpr long long INF=4e18;
	int V=(int)f.size()-1;
	for(int j=w;j<=V;j++){
		if(f[j-w]==-INF) continue;
		f[j]=max(f[j],(long long)((__int128)f[j-w]+v));
	}
}
