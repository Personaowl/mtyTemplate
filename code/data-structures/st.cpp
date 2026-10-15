// Idempotent range maximum; f[i][j] covers [i,i+2^j-1].
struct ST{
	int n;
	vector<int> lg;
	vector<vector<long long>> f;
	void init(const vector<long long>& a){
		n=(int)a.size()-1;lg.assign(n+1,0);
		for(int i=2;i<=n;i++) lg[i]=lg[i/2]+1;
		f.assign(n+1,vector<long long>(lg[n]+1));
		for(int i=1;i<=n;i++) f[i][0]=a[i];
		for(int j=1;j<=lg[n];j++){
			for(int i=1;i+(1LL<<j)-1<=n;i++){
				f[i][j]=max(f[i][j-1],f[i+(1LL<<(j-1))][j-1]);
			}
		}
	}
	long long query(int l,int r)const{
		int k=lg[r-l+1];
		return max(f[l][k],f[r-(1LL<<k)+1][k]);
	}
};
