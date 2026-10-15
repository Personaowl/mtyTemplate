#include<bits/stdc++.h>
#ifdef MTY_INT64
#define int long long
#endif
using namespace std;
#include "code/dp/knapsack.cpp"
#include "code/dp/bounded_knapsack.cpp"
#include "code/dp/group_knapsack.cpp"
#include "code/dp/two_cost_knapsack.cpp"
#include "code/dp/knapsack_count.cpp"
#include "code/dp/knapsack_trace.cpp"
#include "code/dp/interval_merge.cpp"
#include "code/dp/interval_ends.cpp"
#include "code/dp/subset_dp.cpp"
#include "code/dp/hamilton.cpp"
#include "code/dp/profile_dp.cpp"
#include "code/dp/digit_dp.cpp"
#include "code/dp/tree_knapsack.cpp"
#include "code/dp/reroot_dp.cpp"
#include "code/dp/probability_dp.cpp"
#include "code/dp/expectation_dp.cpp"
#include "code/dp/queue_optimization.cpp"
#include "code/dp/stack_optimization.cpp"
#include "code/dp/monotone_cht.cpp"
#include "code/dp/line_container.cpp"
#include "code/dp/divide_conquer.cpp"

constexpr long long INF=4e18;
long long checks=0;
void require(bool ok,const string& msg){
	checks++;if(!ok){cerr<<"FAIL: "<<msg<<'\n';exit(1);}
}
void near(long double x,long double y,const string& msg){
	require(fabsl(x-y)<=1e-13L*max(1.0L,fabsl(y)),msg);
}
struct Item{int w;long long v;int c;};
vector<long long> pack_model(const vector<Item>& items,int V,bool exact){
	vector<long long> ans(V+1,exact?-INF:0);
	auto dfs=[&](auto&& self,int i,int used,long long value)->void{
		if(i==(int)items.size()){
			if(exact) ans[used]=max(ans[used],value);
			else for(int j=used;j<=V;j++) ans[j]=max(ans[j],value);
			return;
		}
		auto [w,v,c]=items[i];
		for(int t=0;t<=c&&used+t*w<=V;t++) self(self,i+1,used+t*w,value+t*v);
	};
	dfs(dfs,0,0,0);return ans;
}
void check_knapsacks(){
	vector<long long> heavy(6,-INF);heavy[0]=0;zero_one(heavy,2,3);auto before=heavy;
	bounded_monotone(heavy,numeric_limits<int>::max(),7,LLONG_MAX);
	require(heavy==before,"bounded weight beyond capacity/index range");
	for(int V=0;V<=15;V++) for(bool exact:{false,true}){
		vector<Item> items={{2,3,1},{3,-2,4},{1,0,3},{4,7,2}};
		vector<long long> binary(V+1,exact?-INF:0);binary[0]=0;
		vector<long long> mono=binary;
		vector<Item> prefix;
		for(auto item:items){
			prefix.push_back(item);
			bounded_knapsack(binary,item.w,item.v,item.c);
			bounded_monotone(mono,item.w,item.v,item.c);
			auto expected=pack_model(prefix,V,exact);
			require(binary==expected,"binary bounded/exact reachability");
			require(mono==expected,"queue bounded/negative values/unreachable states");
		}
		vector<long long> dp(V+1,exact?-INF:0);dp[0]=0;
		zero_one(dp,3,5);complete_knapsack(dp,2,-1);bounded_knapsack(dp,1,2,3);
		require(dp==pack_model({{3,5,1},{2,-1,V/2},{1,2,3}},V,exact),"mixed knapsack orders");
		dp.assign(V+1,exact?-INF:0);dp[0]=0;bounded_knapsack(dp,1,1,3);
		require(dp==pack_model({{1,1,3}},V,exact),"bounded overuse regression");
		bounded_monotone(dp,2,9,0);
		require(dp==pack_model({{1,1,3}},V,exact),"zero count does not change states");
	}
	vector<vector<pair<int,long long>>> groups={{{0,2},{2,4},{3,-1}},{{1,3},{2,8}},{{3,6},{4,9}}};
	for(bool exact:{false,true}){
		int V=9;vector<long long> dp(V+1,exact?-INF:0);dp[0]=0;
		for(const auto& group:groups) group_knapsack(dp,group);
		vector<long long> expected(V+1,exact?-INF:0);
		auto dfs=[&](auto&& self,int i,int used,long long val)->void{
			if(i==(int)groups.size()){
				if(exact) expected[used]=max(expected[used],val);
				else for(int j=used;j<=V;j++) expected[j]=max(expected[j],val);
				return;
			}
			self(self,i+1,used,val);
			for(auto [w,v]:groups[i]) if(used+w<=V) self(self,i+1,used+w,val+v);
		};
		dfs(dfs,0,0,0);require(dp==expected,"group exclusivity incl zero cost");
	}
	vector<tuple<int,int,long long>> items={{0,0,2},{1,2,4},{2,1,3},{2,3,-5},{3,2,7}};
	for(bool exact:{false,true}){
		int V=6,U=6;vector<vector<long long>> dp(V+1,vector<long long>(U+1,exact?-INF:0));dp[0][0]=0;
		for(auto [a,b,v]:items) two_cost_knapsack(dp,a,b,v);
		vector<vector<long long>> ans(V+1,vector<long long>(U+1,exact?-INF:0));
		for(int mask=0;mask<(1LL<<items.size());mask++){
			int a=0,b=0;long long val=0;
			for(int i=0;i<(int)items.size();i++) if(mask>>i&1){
				a+=get<0>(items[i]);b+=get<1>(items[i]);val+=get<2>(items[i]);
			}
			if(a>V||b>U) continue;
			if(exact) ans[a][b]=max(ans[a][b],val);
			else for(int j=a;j<=V;j++) for(int k=b;k<=U;k++) ans[j][k]=max(ans[j][k],val);
		}
		require(dp==ans,"two cost 0/1 subset model");
	}
	vector<int> w={0,1,2,2,4};vector<long long> v={0,-1,3,5,6};
	for(int V=0;V<=10;V++){
		auto [value,chosen]=knapsack_trace(w,v,V);int used=0;long long val=0;
		set<int> unique;
		for(int i:chosen){used+=w[i];val+=v[i];unique.insert(i);}
		vector<Item> items;for(int i=1;i<(int)w.size();i++) items.push_back({w[i],v[i],1});
		require(value==pack_model(items,V,false)[V],"trace optimum");
		require(unique.size()==chosen.size()&&used<=V&&val==value,"trace immutable layers");
		for(bool full:{false,true}) for(long long P:{1LL,2LL,1000000007LL,LLONG_MAX}){
			vector<long long> count(V+1);
			auto dfs=[&](auto&& self,int i,int used)->void{
				if(i==(int)w.size()){count[used]++;return;}
				for(int k=0;k<=(full?V/w[i]:1)&&used+k*w[i]<=V;k++) self(self,i+1,used+k*w[i]);
			};
			dfs(dfs,1,0);for(auto& x:count) x%=P;
			require(knapsack_ways(w,V,P,full)==count,"knapsack combination counts");
		}
	}
	vector<int> ones(71,1);ones[0]=0;
	for(bool full:{false,true}){
		int N=full?104:70,K=35;__int128 c=1;
		for(int i=1;i<=K;i++) c=c*(N-i+1)/i;
		require(knapsack_ways(ones,35,LLONG_MAX,full)[35]==c%LLONG_MAX,"large count/modulo sum");
	}
}
long long merge_model(vector<long long> a,bool ring,map<vector<long long>,long long>& memo){
	if(a.size()==1) return 0;
	if(memo.count(a)) return memo[a];
	long long ans=INF;
	int n=a.size();
	for(int i=0;i<n-(ring?0:1);i++){
		vector<long long> b;long long val=a[i]+a[(i+1)%n];
		if(i==n-1){b.push_back(val);for(int j=1;j<n-1;j++) b.push_back(a[j]);}
		else for(int j=0;j<n;j++){
			if(j==i){b.push_back(val);j++;}else b.push_back(a[j]);
		}
		ans=min(ans,val+merge_model(b,ring,memo));
	}
	return memo[a]=ans;
}
void check_intervals(){
	for(int n=1;n<=6;n++) for(int pattern=0;pattern<4;pattern++){
		vector<long long> a(n+1);for(int i=1;i<=n;i++) a[i]=(i*i+pattern*i)%7;
		for(bool ring:{false,true}){
			map<vector<long long>,long long> memo;
			require(merge_stones(a,ring)==merge_model({a.begin()+1,a.end()},ring,memo),"adjacent merge enumeration");
		}
	}
	for(int n=0;n<=7;n++) for(int mask=0;mask<(1LL<<n);mask++){
		string s;for(int i=0;i<n;i++) s+=char('a'+(mask>>i&1));string t=s;reverse(t.begin(),t.end());
		vector<vector<int>> f(n+1,vector<int>(n+1));
		for(int i=1;i<=n;i++) for(int j=1;j<=n;j++){
			f[i][j]=s[i-1]==t[j-1]?f[i-1][j-1]+1:max(f[i-1][j],f[i][j-1]);
		}
		require(palindrome_insertions(s)==n-f[n][n],"palindrome insertion vs LCS length");
	}
}
long long tiling_model(int n,int m,long long mask){
	int N=n*m,i=0;while(i<N&&(mask>>i&1)) i++;
	if(i==N) return 1;
	long long ans=0;
	if(i%m+1<m&&!(mask>>(i+1)&1)) ans+=tiling_model(n,m,mask|(1LL<<i)|(1LL<<(i+1)));
	if(i+m<N&&!(mask>>(i+m)&1)) ans+=tiling_model(n,m,mask|(1LL<<i)|(1LL<<(i+m)));
	return ans;
}
void check_masks(){
	for(int n=0;n<=6;n++){
		int S=1LL<<n;vector<long long> a(S);for(int i=0;i<S;i++) a[i]=i%7-3;
		auto f=subset_sums(a,n);
		for(int mask=0;mask<S;mask++){
			long long val=0;for(int sub=0;sub<S;sub++) if((sub&mask)==sub) val+=a[sub];
			require(f[mask]==val,"SOS direct subset model");
		}
	}
	for(int n=1;n<=6;n++) for(int pattern=0;pattern<3;pattern++){
		vector<vector<long long>> w(n,vector<long long>(n,INF));
		for(int i=0;i<n;i++) for(int j=0;j<n;j++) if(i!=j&&(i+2*j+pattern)%4){w[i][j]=(i*3+j)%9-3;}
		int start=pattern%n,target=(n-1+pattern)%n;vector<int> order;
		for(int i=0;i<n;i++) if(i!=start) order.push_back(i);
		long long ans=INF;
		do{
			int u=start;long long val=0;bool ok=true;
			for(int v:order){if(w[u][v]==INF){ok=false;break;}val+=w[u][v];u=v;}
			if(ok&&u==target) ans=min(ans,val);
		}while(next_permutation(order.begin(),order.end()));
		require(hamilton_path(w,start,target)==ans,"Hamilton permutation model");
	}
	for(int n=1;n<=4;n++) for(int m=1;m<=5;m++){
		long long ways=tiling_model(n,m,0);
		for(long long P:{1LL,7LL,1000000007LL}) require(domino_tilings(n,m,P)==ways%P,"profile vs board backtracking");
	}
}
bool windy(long long x){
	if(x<=0) return false;
	string s=to_string(x);
	for(int i=1;i<(int)s.size();i++) if(abs(s[i]-s[i-1])<2) return false;
	return true;
}
void check_digits_trees(){
	long long count=0;
	for(int x=0;x<=20000;x++){
		count+=windy(x);
		if(x%13==0||x==9||x==10||x==99||x==100||x==1000) require(count_windy(x)==count,"digit enumeration/leading zero/reset");
	}
	require(windy_range(0,0)==0&&windy_range(0,100)==count_windy(100),"digit zero convention");
	require(count_windy(LLONG_MAX)-count_windy(LLONG_MAX-1)==windy(LLONG_MAX),"digit 64-bit endpoint");
	for(int n=1;n<=8;n++) for(int root:{(int)1,n}){
		vector<vector<int>> e(n+1);
		vector<vector<pair<int,long long>>> weighted(n+1);
		vector<long long> w(n+1);
		for(int i=1;i<=n;i++) w[i]=i%5-2;
		for(int i=2;i<=n;i++){
			e[i].push_back(i/2);e[i/2].push_back(i);
			weighted[i].push_back({i/2,i%3});weighted[i/2].push_back({i,i%3});
		}
		vector<int> fa(n+1),seen(n+1);queue<int> q;q.push(root);seen[root]=1;
		while(!q.empty()){
			int u=q.front();q.pop();for(int v:e[u]) if(!seen[v]){fa[v]=u;seen[v]=1;q.push(v);}
		}
		vector<long long> best(n+1,-INF);best[0]=0;
		for(int mask=1;mask<(1LL<<n);mask++){
			long long val=0;bool ok=true;int cnt=0;
			for(int u=1;u<=n;u++) if(mask>>(u-1)&1){
				if(fa[u]&&!(mask>>(fa[u]-1)&1)) ok=false;
				val+=w[u];cnt++;
			}
			if(ok) best[cnt]=max(best[cnt],val);
		}
		for(int K=0;K<=n;K++){
			auto f=tree_knapsack(e,w,K,root);require(f==vector<long long>(best.begin(),best.begin()+K+1),"tree dependent subset model");
		}
		auto ans=distance_sums(weighted,root);
		for(int u=1;u<=n;u++){
			vector<long long> dis(n+1,-1);q.push(u);dis[u]=0;
			while(!q.empty()){
				int x=q.front();q.pop();for(auto [v,w]:weighted[x]) if(dis[v]==-1){dis[v]=dis[x]+w;q.push(v);}
			}
			require(ans[u]==accumulate(dis.begin()+1,dis.end(),0LL),"reroot all-source distances");
		}
	}
}
void check_probabilities(){
	for(int n=0;n<=7;n++){
		vector<long double> p(n+1);for(int i=1;i<=n;i++) p[i]=(i%5)/4.0L;
		auto f=head_probabilities(p);vector<long double> expected(n+1);
		for(int mask=0;mask<(1LL<<n);mask++){
			long double prob=1;int cnt=0;
			for(int i=0;i<n;i++){if(mask>>i&1){prob*=p[i+1];cnt++;}else prob*=1-p[i+1];}
			expected[cnt]+=prob;
		}
		long double sum=0,mean=0;
		for(int i=0;i<=n;i++){near(f[i],expected[i],"Bernoulli outcome enumeration");sum+=f[i];mean+=i*f[i];}
		near(sum,1,"probability normalization");near(mean,accumulate(p.begin(),p.end(),0.0L),"expectation linearity");
	}
	vector<vector<ProbEdge>> e(4);
	e[1]={{1,0.5L,2},{2,0.3L,1},{3,0.2L,5}};e[2]={{2,0.25L,1},{3,0.75L,3}};
	auto f=expected_cost(e);
	for(int start=1;start<=3;start++){
		vector<long double> mass(4);mass[start]=1;long double ans=0;
		for(int t=0;t<300;t++){
			vector<long double> next(4);
			for(int u=1;u<3;u++) for(auto [v,p,w]:e[u]){ans+=mass[u]*p*w;next[v]+=mass[u]*p;}
			mass.swap(next);
		}
		near(f[start],ans,"expected cost vs converging probability process");
	}
	e.assign(3,{});long double p=1e-18L;e[1]={{1,1-p,1},{2,p,1}};
	near(expected_cost(e)[1],1/p,"small leaving probability");
}
long long partition_model(const vector<long long>& a,int K,bool square){
	int n=(int)a.size()-1;if(n==0) return K==0?0:INF;if(K==0) return INF;
	long long ans=INF;
	for(int mask=0;mask<(1LL<<(n-1));mask++) if(__builtin_popcountll(mask)==K-1){
		long long val=0,cur=square?0:LLONG_MIN;
		for(int i=1;i<=n;i++){
			if(square) cur+=a[i];else cur=max(cur,a[i]);
			if(i==n||(mask>>(i-1)&1)){val+=square?cur*cur:cur;cur=square?0:LLONG_MIN;}
		}
		ans=min(ans,val);
	}
	return ans;
}
vector<long long> quadratic_model(const vector<long long>& a,long long C){
	int n=(int)a.size()-1;vector<long long> f(n+1),s(n+1);
	for(int i=1;i<=n;i++){
		s[i]=s[i-1]+a[i];f[i]=INF;
		for(int j=0;j<i;j++) f[i]=min(f[i],f[j]+(s[i]-s[j])*(s[i]-s[j])+C);
	}
	return f;
}
void check_optimizations(){
	for(int n=0;n<=5;n++){
		int total=1;for(int i=0;i<n;i++) total*=3;
		for(int mask=0;mask<total;mask++){
			vector<long long> a(n+1),positive(n+1);int cur=mask;
			for(int i=1;i<=n;i++){a[i]=cur%3-1;positive[i]=cur%3;cur/=3;}
			for(int K=0;K<=n;K++){
				require(partition_maxima(a,K)==partition_model(a,K,false),"stack optimization vs cut enumeration");
				vector<long long> s(n+1);for(int i=1;i<=n;i++) s[i]=s[i-1]+positive[i];
				auto cost=[&](int l,int r)->long long{return (s[r]-s[l-1])*(s[r]-s[l-1]);};
				require(divide_conquer_dp(n,K,cost)[n]==partition_model(positive,K,true),"divide-conquer vs cuts");
			}
			for(long long C:{-2LL,0LL,3LL}){
				require(quadratic_dp(positive,C)==quadratic_model(positive,C),"monotone quadratic vs direct DP");
				require(quadratic_dp_dynamic(a,C)==quadratic_model(a,C),"dynamic quadratic arbitrary prefix order");
			}
			for(int k=1;k<=max((int)1,n);k++){
				auto f=window_dp(a,k);vector<long long> direct(n+1);
				for(int i=1;i<=n;i++){
					direct[i]=-INF;for(int j=max((int)0,i-k);j<i;j++) direct[i]=max(direct[i],direct[j]+a[i]);
				}
				require(f==direct,"sliding DP timing/boundary");
			}
		}
	}
}
__int128 line_value(pair<long long,long long> l,long long x){return (__int128)l.first*x+l.second;}
void check_hulls(){
	vector<vector<pair<long long,long long>>> groups={
		{{0,LLONG_MAX},{1,LLONG_MIN}},{{0,LLONG_MIN},{1,LLONG_MAX}},
		{{LLONG_MIN,LLONG_MAX},{LLONG_MAX,LLONG_MIN},{0,LLONG_MIN},{0,LLONG_MAX},
		 {-1,5},{1,-5},{3,2},{3,7},{-2,LLONG_MIN},{3,7}},
		{{3,2},{-1,7},{5,-11},{3,9},{0,-2},{-4,6},{2,-13},{2,8},{2,-1}}
	};
	vector<long long> xs={LLONG_MIN,LLONG_MAX,-1000000000000LL,7,-4,0,1,-1,2,19};
	for(auto lines:groups){
		LineContainer hull;vector<pair<long long,long long>> old;
		for(auto [k,m]:lines){
			hull.add_line(k,m);old.push_back({k,m});
			for(long long x:xs){
				__int128 best=line_value(old[0],x);for(auto l:old) best=max(best,line_value(l,x));
				require(hull.query(x)==best,"dynamic hull full signed range/equal slopes/floor boundaries");
			}
		}
		require(hull.div_floor(-1,2)==-1&&hull.div_floor(1,-2)==-1&&hull.div_floor(-4,-2)==2,"floor division signs");
		hull.clear();hull.add_line(0,-9);require(hull.query(LLONG_MIN)==-9,"dynamic hull clear");
	}
	MonotoneCHT hull;vector<pair<long long,long long>> lines;
	for(int i=0;i<55;i++){
		long long k=i/3-9,m=(i*11)%23-11,x=i/2-13;
		hull.add_line(k,m);lines.push_back({k,m});
		__int128 best=line_value(lines[0],x);for(auto l:lines) best=max(best,line_value(l,x));
		require(hull.query(x)==best,"monotone hull interleaved ties/query order");
	}
	hull.init();lines={{-1000000000000000000LL,1000000000000000000LL},{-1,-7},{-1,5},{0,0},
		{1000000000000000000LL,-1000000000000000000LL}};
	for(auto [k,m]:lines) hull.add_line(k,m);
	for(long long x:{-1000000000000000000LL,-1LL,0LL,1LL,1000000000000000000LL}){
		__int128 best=line_value(lines[0],x);for(auto l:lines) best=max(best,line_value(l,x));
		require(hull.query(x)==best,"monotone cross products/large coefficients");
	}
	// Minimum queries use both negated coefficients; the original slopes decrease.
	MonotoneCHT lower;
	LineContainer dynamic_lower;
	vector<pair<long long,long long>> original;
	for(int i=0;i<45;i++){
		long long k=12-i/2,m=(i*17)%29-14,x=i/3-9;
		original.push_back({k,m});lower.add_line(-k,-m);
		dynamic_lower.add_line(-k,-m);
		__int128 best=line_value(original[0],x);
		for(auto l:original) best=min(best,line_value(l,x));
		require(-lower.query(x)==best,"monotone minimum negation/decreasing original slopes");
		for(long long qx:{LLONG_MIN,LLONG_MAX,7LL,-4LL,0LL}){
			__int128 value=line_value(original[0],qx);
			for(auto l:original) value=min(value,line_value(l,qx));
			require(-dynamic_lower.query(qx)==value,"dynamic minimum negation/arbitrary x");
		}
	}

}
signed main(){
	check_knapsacks();check_intervals();check_masks();check_digits_trees();
	check_probabilities();check_optimizations();check_hulls();
	cout<<"PASS: DP, "<<checks<<" fixed independent-model/boundary checks\n";
}
