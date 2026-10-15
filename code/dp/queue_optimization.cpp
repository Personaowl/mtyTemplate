// f[i]=a[i]+max f[j], max(0,i-k)<=j<i; f[0]=0.
vector<long long> window_dp(const vector<long long>& a,int k){
	int n=(int)a.size()-1;vector<long long> f(n+1);deque<int> q{0};
	for(int i=1;i<=n;i++){
		while(!q.empty()&&q.front()<i-k) q.pop_front();
		f[i]=(__int128)f[q.front()]+a[i];
		while(!q.empty()&&f[q.back()]<=f[i]) q.pop_back();
		q.push_back(i);
	}
	return f;
}
// One bounded item in O(V), always using the previous layer.
void bounded_monotone(vector<long long>& f,int w,long long v,long long cnt){
	static constexpr long long INF=4e18;
	int V=(int)f.size()-1;
	auto old=f;cnt=min(cnt,(long long)(V/w));
	for(int r=0;r<w&&r<=V;r++){
		deque<pair<int,__int128>> q;
		int last=(V-r)/w;
		for(int t=0;t<=last;t++){
			int pos=r+t*w;
			while(!q.empty()&&q.front().first<t-cnt) q.pop_front();
			if(old[pos]!=-INF){
				__int128 val=(__int128)old[pos]-(__int128)t*v;
				while(!q.empty()&&q.back().second<=val) q.pop_back();
				q.push_back({t,val});
			}
			f[pos]=q.empty()?-INF:q.front().second+(__int128)t*v;
		}
	}
}
