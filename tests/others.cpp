long long checks=0;
void require(bool ok){
	checks++;if(!ok){cerr<<"Others check failed #"<<checks<<'\n';exit(1);}
}
signed main(){
	mt19937_64 rng(20261015);
	for(long long c=-10;c<=10;c++){
		auto f=[&](long double x){return (x-c)*(x-c)+5;};
		long double x=ternary_min(-20,20,f);
		require(fabsl(x-c)<1e-7L);
		require(ternary_min_int(-20,20,[&](long long t){return (t-c)*(t-c);})==c);
		require(first_true_real(-20,20,[&](long double t){return t>=c;})>=c);
		require(fabsl(first_true_real(-20,20,[&](long double t){return t>=c;})-c)<1e-12L);
		require(last_true_real(-20,20,[&](long double t){return t<=c;})<=c);
	}
	require(ternary_min_int(7,7,[](long long x){return x*x;})==7);
	require(fabsl(ternary_min(0,5,[](long double x){return x;})-0)<1e-12L);
	require(fabsl(ternary_min(0,5,[](long double x){return -x;})-5)<1e-12L);
	long double flat=ternary_min(-10,10,[](long double x){return max(0.0L,fabsl(x)-2);});
	require(fabsl(flat)<=2+1e-12L);
	require(subset_sum_count({},0)==1&&subset_sum_count({},1)==0);
	require(subset_sum_count(vector<long long>(40,0),0)==(1LL<<40));
	require(subset_sum_count({4000000000000000000LL},LLONG_MIN)==0);
	for(int t=0;t<120;t++){
		int n=rng()%17;vector<long long> a(n);
		for(auto& x:a) x=(long long)(rng()%15)-7;
		long long target=(long long)(rng()%31)-15,expected=0;
		for(long long mask=0;mask<(1LL<<n);mask++){
			long long sum=0;for(int i=0;i<n;i++) if(mask>>i&1) sum+=a[i];
			expected+=sum==target;
		}
		require(subset_sum_count(a,target)==expected);
	}
	for(int t=0;t<70;t++){
		int n=1+rng()%6,m=1+rng()%6;
		vector<vector<long long>> a(n+1,vector<long long>(m+1));
		for(int i=1;i<=n;i++) for(int j=1;j<=m;j++) a[i][j]=(long long)(rng()%21)-10;
		Prefix2D pre;pre.build(a);
		Diff2D diff;diff.init(n,m);
		auto expected=vector<vector<long long>>(n+1,vector<long long>(m+1));
		for(int x1=1;x1<=n;x1++) for(int x2=x1;x2<=n;x2++){
			for(int y1=1;y1<=m;y1++) for(int y2=y1;y2<=m;y2++){
				long long sum=0,v=(long long)(rng()%11)-5;
				for(int i=x1;i<=x2;i++) for(int j=y1;j<=y2;j++){
					sum+=a[i][j];expected[i][j]+=v;
				}
				require(pre.query(x1,y1,x2,y2)==sum);
				diff.add(x1,y1,x2,y2,v);
			}
		}
		require(diff.build()==expected);require(diff.build()==expected);
		diff.add(1,1,n,m,2);
		for(int i=1;i<=n;i++) for(int j=1;j<=m;j++) expected[i][j]+=2;
		require(diff.build()==expected);
		diff.init(n,m);require(diff.build()==vector<vector<long long>>(n+1,vector<long long>(m+1)));
	}
	for(int t=0;t<200;t++){
		int d=1+rng()%4,V=1;vector<int> shape(d),step(d,1),lo(d),hi(d);
		for(auto& v:shape){v=1+rng()%4;V*=v;}
		for(int k=d-2;k>=0;k--) step[k]=step[k+1]*shape[k+1];
		vector<long long> a(V);for(auto& v:a) v=(long long)(rng()%21)-10;
		auto s=multi_prefix(a,shape);
		for(int q=0;q<20;q++){
			for(int k=0;k<d;k++){
				lo[k]=rng()%shape[k];hi[k]=rng()%shape[k];if(lo[k]>hi[k]) swap(lo[k],hi[k]);
			}
			long long expected=0;
			for(int id=0;id<V;id++){
				bool ok=true;
				for(int k=0;k<d;k++){int x=id/step[k]%shape[k];ok&=lo[k]<=x&&x<=hi[k];}
				if(ok) expected+=a[id];
			}
			require(multi_sum(s,shape,lo,hi)==expected);
		}
	}
	cout<<"Others: "<<checks<<" checks passed\n";
}
