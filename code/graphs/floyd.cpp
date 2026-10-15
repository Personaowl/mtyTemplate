struct Floyd{
	static constexpr long long INF=4e18;
	int n;
	vector<vector<long long>> dis;
	void init(int n){
		this->n=n;
		dis.assign(n+1,vector<long long>(n+1,INF));
		for(int i=1;i<=n;i++) dis[i][i]=0;
	}
	void add(int u,int v,long long w){dis[u][v]=min(dis[u][v],w);}
	bool run(){
		for(int k=1;k<=n;k++){
			for(int i=1;i<=n;i++){
				if(dis[i][k]==INF) continue;
				for(int j=1;j<=n;j++){
					if(dis[k][j]==INF) continue;
					__int128 v=(__int128)dis[i][k]+dis[k][j];
					if(v<dis[i][j]) dis[i][j]=max(v,-(__int128)INF);
				}
			}
		}
		for(int i=1;i<=n;i++) if(dis[i][i]<0) return false;
		return true;
	}
};
