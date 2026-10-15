// Dependency: knapsack.cpp. Binary bundles are only used for maximizing value.
void bounded_knapsack(vector<long long>& f,int w,long long v,long long cnt){
	int V=(int)f.size()-1;
	cnt=min(cnt,(long long)(V/w));
	for(long long k=1;cnt>0;k<<=1){
		long long take=min(k,cnt);cnt-=take;
		zero_one(f,w*take,(__int128)v*take);
	}
}
