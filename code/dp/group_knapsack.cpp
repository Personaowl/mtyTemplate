// Each group contributes at most one item; every choice reads the old layer.
void group_knapsack(vector<long long>& f,const vector<pair<int,long long>>& items){
	static constexpr long long INF=4e18;
	int V=(int)f.size()-1;
	auto old=f;
	for(int j=0;j<=V;j++){
		for(auto [w,v]:items) if(w<=j&&old[j-w]!=-INF){
			f[j]=max(f[j],(long long)((__int128)old[j-w]+v));
		}
	}
}
