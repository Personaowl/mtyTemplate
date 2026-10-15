int gauss(vector<vector<double>> a,vector<double> &ans,double eps=1e-9){
	int n=a.size(),m=(int)a[0].size()-1,r=0;
	vector<int> pos(m,-1);
	for(int c=0;c<m&&r<n;c++){
		int p=r;
		for(int i=r+1;i<n;i++){
			if(abs(a[i][c])>abs(a[p][c])) p=i;
		}
		if(abs(a[p][c])<=eps) continue;
		swap(a[p],a[r]);
		double v=a[r][c];
		for(int j=c;j<=m;j++) a[r][j]/=v;
		for(int i=0;i<n;i++){
			if(i==r) continue;
			double t=a[i][c];
			for(int j=c;j<=m;j++) a[i][j]-=t*a[r][j];
		}
		pos[c]=r++;
	}
	ans.assign(m,0);
	for(int i=r;i<n;i++){
		if(abs(a[i][m])>eps){ans.clear();return 0;}
	}
	for(int c=0;c<m;c++) if(pos[c]!=-1) ans[c]=a[pos[c]][m];
	return r==m?1:2;
}
