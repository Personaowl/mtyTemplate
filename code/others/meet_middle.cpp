// 枚举 0 下标半开区间 [l,r) 的子集和，保留重复值和空集。
vector<long long> half_sums(const vector<long long>& a,int l,int r){
	vector<long long> s;
	auto dfs=[&](auto&& self,int i,long long sum)->void{
		if(i==r){s.push_back(sum);return;}
		self(self,i+1,sum);
		self(self,i+1,sum+a[i]);
	};
	dfs(dfs,l,0);return s;
}
// 统计和等于 target 的子集个数；n<=40，所有元素绝对值之和<=4e18。
long long subset_sum_count(const vector<long long>& a,long long target){
	int n=a.size(),mid=n/2;
	auto l=half_sums(a,0,mid),r=half_sums(a,mid,n);
	sort(r.begin(),r.end());long long ans=0;
	for(auto s:l){
		__int128 need=(__int128)target-s;
		auto range=equal_range(r.begin(),r.end(),need);
		ans+=range.second-range.first;
	}
	return ans;
}
