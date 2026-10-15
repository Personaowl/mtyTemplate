// Exactly K nonempty segments, minimizing the sum of segment maxima.
long long partition_maxima(const vector<long long>& a,int K){
	static constexpr long long INF=4e18;
	struct Node{long long maxn,best,pref;};
	int n=(int)a.size()-1;vector<long long> f(n+1,INF);f[0]=0;
	for(int layer=1;layer<=K;layer++){
		vector<long long> g(n+1,INF);vector<Node> st;
		for(int i=1;i<=n;i++){
			long long best=f[i-1];
			while(!st.empty()&&st.back().maxn<=a[i]){
				best=min(best,st.back().best);st.pop_back();
			}
			long long val=best==INF?INF:(long long)((__int128)best+a[i]);
			if(!st.empty()) val=min(val,st.back().pref);
			st.push_back({a[i],best,val});g[i]=val;
		}
		f.swap(g);
	}
	return f[n];
}
