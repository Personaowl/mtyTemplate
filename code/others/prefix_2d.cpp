struct Prefix2D{
	int n,m;
	vector<vector<long long>> s;
	void build(const vector<vector<long long>>& a){
		n=(int)a.size()-1;m=(int)a[0].size()-1;
		s.assign(n+1,vector<long long>(m+1));
		for(int i=1;i<=n;i++) for(int j=1;j<=m;j++){
			s[i][j]=(__int128)a[i][j]+s[i-1][j]+s[i][j-1]-s[i-1][j-1];
		}
	}
	long long query(int x1,int y1,int x2,int y2)const{
		return (__int128)s[x2][y2]-s[x1-1][y2]-s[x2][y1-1]+s[x1-1][y1-1];
	}
};
