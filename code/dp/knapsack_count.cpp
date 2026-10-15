// Order-independent selections, not permutations. Every input entry is one type/item.
vector<long long> knapsack_ways(const vector<int>& w,int V,long long P,bool complete=false){
	vector<long long> f(V+1);f[0]=1%P;
	for(int i=1;i<(int)w.size();i++){
		if(complete){
			for(int j=w[i];j<=V;j++) f[j]=((__int128)f[j]+f[j-w[i]])%P;
		}
		else{
			for(int j=V;j>=w[i];j--) f[j]=((__int128)f[j]+f[j-w[i]])%P;
		}
	}
	return f;
}
