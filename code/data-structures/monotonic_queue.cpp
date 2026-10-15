// 1-based input and output: index j answers the window [j,j+k-1].
pair<vector<long long>,vector<long long>> window_minmax(
		const vector<long long>& a,int k){
	int n=(int)a.size()-1;
	vector<long long> minn(n-k+2),maxn(n-k+2);
	deque<int> q1,q2;
	for(int i=1;i<=n;i++){
		while(!q1.empty()&&q1.front()<=i-k) q1.pop_front();
		while(!q2.empty()&&q2.front()<=i-k) q2.pop_front();
		while(!q1.empty()&&a[q1.back()]>=a[i]) q1.pop_back();
		while(!q2.empty()&&a[q2.back()]<=a[i]) q2.pop_back();
		q1.push_back(i);q2.push_back(i);
		if(i>=k){
			minn[i-k+1]=a[q1.front()];maxn[i-k+1]=a[q2.front()];
		}
	}
	return {move(minn),move(maxn)};
}
