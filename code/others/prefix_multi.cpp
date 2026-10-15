// shape 给出各维长度，末维变化最快；a 不带 1 下标占位。
vector<long long> multi_prefix(vector<long long> a,const vector<int>& shape){
	long long step=1;
	for(int k=(int)shape.size()-1;k>=0;k--){
		for(long long id=0;id<(long long)a.size();id++){
			if(id/step%shape[k]) a[id]+=a[id-step];
		}
		step*=shape[k];
	}
	return a;
}
// 查询各维闭区间 [lo[k],hi[k]] 的笛卡尔积，0 下标。
long long multi_sum(const vector<long long>& s,const vector<int>& shape,
	const vector<int>& lo,const vector<int>& hi){
	int d=shape.size();vector<long long> step(d,1);
	for(int k=d-2;k>=0;k--) step[k]=step[k+1]*shape[k+1];
	__int128 ans=0;
	for(long long mask=0;mask<(1LL<<d);mask++){
		long long id=0;int sign=1;bool ok=true;
		for(int k=0;k<d;k++){
			int x=hi[k];
			if(mask>>k&1){x=lo[k]-1;sign=-sign;}
			if(x<0){ok=false;break;}
			id+=x*step[k];
		}
		if(ok) ans+=(__int128)sign*s[id];
	}
	return ans;
}
