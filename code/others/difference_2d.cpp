struct Diff2D{
	int n,m;
	vector<vector<long long>> d;
	void init(int n,int m){
		this->n=n;this->m=m;d.assign(n+2,vector<long long>(m+2));
	}
	void add(int x1,int y1,int x2,int y2,long long v){
		d[x1][y1]+=v;d[x2+1][y1]-=v;
		d[x1][y2+1]-=v;d[x2+1][y2+1]+=v;
	}
	vector<vector<long long>> build()const{
		vector<vector<long long>> a(n+1,vector<long long>(m+1));
		for(int i=1;i<=n;i++) for(int j=1;j<=m;j++){
			a[i][j]=(__int128)d[i][j]+a[i-1][j]+a[i][j-1]-a[i-1][j-1];
		}
		return a;
	}
};
