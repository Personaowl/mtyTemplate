vector<int> prefix_function(const string &s){
	int n=s.size();
	vector<int> pi(n+1,0);
	for(int i=2,j=0;i<=n;i++){
		while(j&&s[i-1]!=s[j]) j=pi[j];
		if(s[i-1]==s[j]) j++;
		pi[i]=j;
	}
	return pi;
}
vector<int> kmp(const string &s,const string &t){
	int n=s.size(),m=t.size();
	vector<int> ans;
	if(m==0){
		ans.resize(n+1);
		iota(ans.begin(),ans.end(),1);
		return ans;
	}
	auto pi=prefix_function(t);
	for(int i=1,j=0;i<=n;i++){
		while(j&&s[i-1]!=t[j]) j=pi[j];
		if(s[i-1]==t[j]) j++;
		if(j==m){
			ans.push_back(i-m+1);
			j=pi[j];
		}
	}
	return ans;
}
