// Independent Bernoulli trials; p[0] is unused.
vector<long double> head_probabilities(const vector<long double>& p){
	int n=(int)p.size()-1;vector<long double> f(n+1);f[0]=1;
	for(int i=1;i<=n;i++) for(int j=i;j>=0;j--){
		f[j]=f[j]*(1-p[i])+(j?f[j-1]*p[i]:0);
	}
	return f;
}
