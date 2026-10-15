// Count positive integers in [1,x] with |adjacent decimal digits| >= 2.
long long count_windy(long long x){
	if(x<=0) return 0;
	string s=to_string(x);
	vector<array<long long,11>> f(s.size());
	for(auto& row:f) row.fill(-1);
	auto dfs=[&](auto&& self,int pos,int pre,bool limit)->long long{
		if(pos==(int)s.size()) return pre!=10;
		if(!limit&&f[pos][pre]!=-1) return f[pos][pre];
		int up=limit?s[pos]-'0':9;
		long long ans=0;
		for(int d=0;d<=up;d++){
			bool next=limit&&d==up;
			if(pre==10&&d==0) ans+=self(self,pos+1,10,next);
			else if(pre==10||abs(pre-d)>=2) ans+=self(self,pos+1,d,next);
		}
		if(!limit) f[pos][pre]=ans;
		return ans;
	};
	return dfs(dfs,0,10,true);
}
long long windy_range(long long l,long long r){
	return count_windy(r)-(l==0?0:count_windy(l-1));
}
