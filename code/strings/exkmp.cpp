vector<int> z_function(const string &s){
	int n=s.size();
	vector<int> z(n+1,0);
	if(n==0) return z;
	z[1]=n;
	for(int i=2,l=0,r=0;i<=n;i++){
		if(i<=r) z[i]=min(r-i+1,z[i-l+1]);
		while(i+z[i]<=n&&s[z[i]]==s[i+z[i]-1]) z[i]++;
		if(i+z[i]-1>r){
			l=i;
			r=i+z[i]-1;
		}
	}
	return z;
}
vector<int> exkmp(const string &s,const string &t){
	int n=s.size(),m=t.size();
	vector<int> e(n+1,0);
	if(m==0) return e;
	auto z=z_function(t);
	for(int i=1,l=0,r=0;i<=n;i++){
		if(i<=r) e[i]=min(r-i+1,z[i-l+1]);
		while(e[i]<m&&i+e[i]<=n&&s[i+e[i]-1]==t[e[i]]) e[i]++;
		if(i+e[i]-1>r){
			l=i;
			r=i+e[i]-1;
		}
	}
	return e;
}
