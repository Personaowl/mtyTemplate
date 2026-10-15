struct Manacher{
	vector<int> p;
	int len=0,pos=1;
	void init(const string &s){
		int n=s.size(),m=2*n+1;
		vector<int> t(m,0);
		for(int i=0;i<n;i++) t[2*i+1]=(unsigned char)s[i]+1;
		p.assign(m,1);
		len=0,pos=1;
		for(int i=0,l=0,r=-1;i<m;i++){
			if(i<=r) p[i]=min(p[l+r-i],r-i+1);
			while(i-p[i]>=0&&i+p[i]<m&&t[i-p[i]]==t[i+p[i]]){
				p[i]++;
			}
			if(i+p[i]-1>r){
				l=i-p[i]+1;
				r=i+p[i]-1;
			}
			if(p[i]-1>len){
				len=p[i]-1;
				pos=(i-len)/2+1;
			}
		}
	}
	bool check(int l,int r){
		if(l>r) return true;
		return p[l+r-1]>=r-l+2;
	}
	long long count(){
		long long res=0;
		for(auto x:p) res+=x/2;
		return res;
	}
};
