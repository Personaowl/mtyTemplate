struct MinCycle{
	static constexpr long long INF=4e18;
	int n;
	bool small;
	long long best;
	vector<vector<long long>> w;
	void init(int n,bool small=true){
		this->n=n;this->small=small;best=INF;
		w.assign(n+1,vector<long long>(n+1,INF));
	}
	void add(int u,int v,long long c){
		if(u==v){if(small) best=min(best,c);return;}
		if(small&&w[u][v]!=INF){
			__int128 z=(__int128)c+w[u][v];
			if(z<best) best=z;
		}
		w[u][v]=w[v][u]=min(w[u][v],c);
	}
	long long run() const{
		auto d=w;
		for(int i=1;i<=n;i++) d[i][i]=0;
		long long ans=best;
		for(int k=1;k<=n;k++){
			for(int i=1;i<k;i++) for(int j=i+1;j<k;j++){
				if(d[i][j]==INF||w[i][k]==INF||w[k][j]==INF) continue;
				__int128 v=(__int128)d[i][j]+w[i][k]+w[k][j];
				if(v<ans) ans=v;
			}
			for(int i=1;i<=n;i++) for(int j=1;j<=n;j++){
				if(d[i][k]==INF||d[k][j]==INF) continue;
				__int128 v=(__int128)d[i][k]+d[k][j];
				if(v<d[i][j]) d[i][j]=v;
			}
		}
		return ans;
	}
};
