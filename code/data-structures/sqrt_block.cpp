struct SqrtBlock{
	int n,len,tot;
	vector<int> bel,L,R;
	vector<long long> a,tag;
	vector<vector<long long>> b;
	void rebuild(int x){
		b[x].assign(a.begin()+L[x],a.begin()+R[x]+1);
		sort(b[x].begin(),b[x].end());
	}
	void init(const vector<long long>& w){
		a=w;n=(int)a.size()-1;len=max((int)1,(int)sqrt(n));
		tot=(n+len-1)/len;bel.assign(n+1,0);
		L.resize(tot+1);R.resize(tot+1);tag.assign(tot+1,0);b.assign(tot+1,{});
		for(int x=1;x<=tot;x++){
			L[x]=(x-1)*len+1;R[x]=min(n,x*len);
			for(int i=L[x];i<=R[x];i++) bel[i]=x;
			rebuild(x);
		}
	}
	void modify(int l,int r,long long val){
		for(int x=bel[l];x<=bel[r];x++){
			if(l<=L[x]&&R[x]<=r) tag[x]+=val;
			else{
				for(int i=max(l,L[x]);i<=min(r,R[x]);i++) a[i]+=val;
				rebuild(x);
			}
		}
	}
	int count(int l,int r,long long val)const{
		int res=0;
		for(int x=bel[l];x<=bel[r];x++){
			if(l<=L[x]&&R[x]<=r){
				__int128 key=(__int128)val-tag[x];
				res+=upper_bound(b[x].begin(),b[x].end(),key,
					[](__int128 v,long long y){return v<y;})-b[x].begin();
			}
			else for(int i=max(l,L[x]);i<=min(r,R[x]);i++){
				res+=(__int128)a[i]+tag[x]<=val;
			}
		}
		return res;
	}
	long long query(int l,int r,int k)const{
		long long lo=LLONG_MAX,hi=LLONG_MIN;
		for(int x=bel[l];x<=bel[r];x++){
			lo=min(lo,b[x].front()+tag[x]);hi=max(hi,b[x].back()+tag[x]);
		}
		while(lo<hi){
			long long mid=(__int128)lo+((__int128)hi-lo)/2;
			if(count(l,r,mid)>=k) hi=mid;
			else lo=mid+1;
		}
		return lo;
	}
};
