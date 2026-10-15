struct ProbEdge{int to;long double p,w;};
// State n is absorbing. Other edges satisfy to>=u, with positive probability to leave u.
vector<long double> expected_cost(const vector<vector<ProbEdge>>& e){
	int n=(int)e.size()-1;vector<long double> f(n+1);
	for(int u=n-1;u>=1;u--){
		long double res=0,leave=0;
		for(auto [v,p,w]:e[u]){
			res+=p*w;
			if(v!=u){res+=p*f[v];leave+=p;}
		}
		f[u]=res/leave;
	}
	return f;
}
