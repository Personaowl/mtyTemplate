int xor_gauss(vector<vector<int>> a,vector<int> &ans,int &rank){
	int n=a.size(),m=(int)a[0].size()-1,r=0;
	vector<int> pos(m,-1);
	for(int c=0;c<m&&r<n;c++){
		int p=r;
		while(p<n&&!a[p][c]) p++;
		if(p==n) continue;
		swap(a[p],a[r]);
		for(int i=0;i<n;i++){
			if(i==r||!a[i][c]) continue;
			for(int j=c;j<=m;j++) a[i][j]^=a[r][j];
		}
		pos[c]=r++;
	}
	rank=r;
	ans.assign(m,0);
	for(int i=r;i<n;i++){
		if(a[i][m]){ans.clear();return 0;}
	}
	for(int c=0;c<m;c++) if(pos[c]!=-1) ans[c]=a[pos[c]][m];
	return r==m?1:2;
}
