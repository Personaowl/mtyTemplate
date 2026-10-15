#include<bits/stdc++.h>
#ifdef MTY_INT64
#define int long long
#endif
using namespace std;
#include "code/data-structures/binary_trie.cpp"
#include "code/graphs/zero_one_bfs.cpp"
#include "code/graphs/euler_walk.cpp"
#include "code/graphs/point_bcc.cpp"
#include "code/number-theory/sieve.cpp"
#include "code/number-theory/mobius.cpp"

long long checks=0;
mt19937_64 rng(20261015);
void require(bool ok,const string& what){
	checks++;
	if(!ok){cerr<<"FAIL: "<<what<<endl;exit(1);}
}

void check_trie(){
	for(int B:vector<int>{1,8,31,63,64}){
		BinaryTrie tr(B);
		multiset<uint64_t> a;
		uint64_t mask=B==64?~0ULL:(1ULL<<B)-1;
		for(int step=0;step<5000;step++){
			uint64_t x=rng()&mask;
			if(step%11==0) x=0;
			if(step%13==0) x=mask;
			int op=rng()%4;
			if(op==0){tr.insert(x);a.insert(x);}
			if(op==1){
				auto it=a.find(x);bool found=it!=a.end();
				require(tr.erase(x)==found,"Trie erase");
				if(found) a.erase(it);
			}
			require(tr.count(x)==(int)a.count(x),"Trie multiplicity");
			require(tr.size()==(int)a.size(),"Trie size");
			for(bool maxn:{false,true}){
				auto [ok,val]=tr.query(x,maxn);
				require(ok==!a.empty(),"Trie empty flag");
				if(ok){
					uint64_t best=maxn?0:~0ULL;
					for(auto y:a) best=maxn?max(best,x^y):min(best,x^y);
					require(val==best&&a.count(x^val),"Trie XOR greedy");
				}
			}
		}
		while(!a.empty()){
			uint64_t x=*a.begin();a.erase(a.begin());
			require(tr.erase(x),"Trie erase all");
		}
		require(tr.size()==0&&!tr.query(mask).first&&!tr.erase(mask),"Trie dead nodes");
		tr.insert(mask);tr.init(B);
		require(tr.size()==0&&tr.count(mask)==0,"Trie reinit");
	}
	BinaryTrie full(64);full.insert(0);full.insert(1ULL<<63);full.insert(~0ULL);
	require(full.query(0).second==~0ULL,"Trie high bit");
	require(full.query(~0ULL,false).second==0,"Trie zero XOR");
	full.erase(~0ULL);full.erase(1ULL<<63);
	require(full.query(1ULL<<63).second==(1ULL<<63),"Trie skip deleted branch");
}

void verify_bfs(int n,const vector<array<int,3>>& edges){
	ZeroOneBFS g;g.init(n);
	vector<vector<long long>> d(n+1,vector<long long>(n+1,1000000));
	for(int i=1;i<=n;i++) d[i][i]=0;
	for(auto [u,v,w]:edges){g.add(u,v,w);d[u][v]=min(d[u][v],(long long)w);}
	for(int k=1;k<=n;k++) for(int i=1;i<=n;i++) for(int j=1;j<=n;j++)
		d[i][j]=min(d[i][j],d[i][k]+d[k][j]);
	for(int s=1;s<=n;s++){
		g.run(s);
		for(int v=1;v<=n;v++) require(g.dis[v]==(d[s][v]==1000000?g.INF:d[s][v]),"0-1 BFS vs Floyd");
	}
	g.init(1);g.run(1);require(g.dis[1]==0&&g.e[1].empty(),"0-1 BFS reinit");
}

void check_bfs(){
	vector<pair<int,int>> arcs;
	for(int u=1;u<=3;u++) for(int v=1;v<=3;v++) if(u!=v) arcs.push_back({u,v});
	for(int mask=0;mask<729;mask++){
		int z=mask;vector<array<int,3>> edges;
		for(auto [u,v]:arcs){int t=z%3;z/=3;if(t) edges.push_back({u,v,t-1});}
		verify_bfs(3,edges);
	}
	for(int t=0;t<500;t++){
		int n=rng()%8+1;vector<array<int,3>> edges;
		for(int i=0,m=rng()%50;i<m;i++) edges.push_back({(int)(rng()%n+1),(int)(rng()%n+1),(int)(rng()%2)});
		verify_bfs(n,edges);
	}
}

bool brute_euler(int n,bool directed,const vector<pair<int,int>>& edges,int start){
	int m=edges.size();
	auto dfs=[&](auto &&self,int u,int used)->bool{
		if(used==(1LL<<m)-1) return true;
		for(int i=0;i<m;i++) if(!(used>>i&1)){
			auto [a,b]=edges[i];
			if(a==u&&self(self,b,used|(1LL<<i))) return true;
			if(!directed&&b==u&&self(self,a,used|(1LL<<i))) return true;
		}
		return false;
	};
	if(start) return dfs(dfs,start,0);
	for(int u=1;u<=n;u++) if(dfs(dfs,u,0)) return true;
	return false;
}

void verify_euler(int n,bool directed,const vector<pair<int,int>>& edges){
	EulerWalk g;g.init(n,directed);
	for(int i=0;i<(int)edges.size();i++) require(g.add(edges[i].first,edges[i].second)==i,"Euler edge IDs");
	for(int s=0;s<=n;s++){
		bool expected=brute_euler(n,directed,edges,s);
		for(int repeat=0;repeat<2;repeat++){
			bool ok=g.build(s);require(ok==expected,"Euler existence vs edge DFS");
			if(!ok){require(g.path.empty()&&g.eid.empty(),"Euler failure clears");continue;}
			require(g.path.size()==edges.size()+1&&g.eid.size()==edges.size(),"Euler length");
			if(s) require(g.path[0]==s,"Euler fixed start");
			vector<bool> seen(edges.size());
			for(int i=0;i<(int)edges.size();i++){
				int id=g.eid[i],u=g.path[i],v=g.path[i+1];
				require(id>=0&&id<(int)edges.size()&&!seen[id],"Euler unique original edge");seen[id]=true;
				auto [a,b]=edges[id];
				require((a==u&&b==v)||(!directed&&a==v&&b==u),"Euler edge-vertex alignment");
			}
		}
	}
	require(!g.build(n+1)&&g.path.empty()&&g.eid.empty(),"Euler invalid start");
	g.init(1,directed);require(g.build()&&g.path==vector<int>{1},"Euler reinit and empty");
}

void check_euler(){
	for(bool directed:{false,true}){
		vector<pair<int,int>> choices,edges;
		for(int u=1;u<=3;u++) for(int v=1;v<=3;v++) if(directed||u<=v) choices.push_back({u,v});
		auto enumerate=[&](auto &&self,int first,int left)->void{
			verify_euler(3,directed,edges);
			if(!left) return;
			for(int i=first;i<(int)choices.size();i++){
				edges.push_back(choices[i]);self(self,i,left-1);edges.pop_back();
			}
		};
		enumerate(enumerate,0,4);
		for(int t=0;t<400;t++){
			int n=rng()%5+1;edges.clear();
			for(int i=0,m=rng()%8;i<m;i++) edges.push_back({rng()%n+1,rng()%n+1});
			verify_euler(n,directed,edges);
		}
		verify_euler(4,directed,{{1,2},{2,1},{3,4},{4,3}});
	}
}

int components(int mask,const vector<int>& adj){
	int res=0;
	while(mask){
		res++;int seen=mask&-mask,front=seen;
		while(front){
			int next=0;
			for(int i=0;i<(int)adj.size();i++) if(front>>i&1) next|=adj[i];
			next&=mask&~seen;seen|=next;front=next;
		}
		mask&=~seen;
	}
	return res;
}

void verify_bcc(int n,const vector<pair<int,int>>& edges){
	PointBCC g;g.init(n);vector<int> adj(n);
	for(auto [u,v]:edges){
		int id=g.add(u,v);
		if(u==v){require(id==-1,"BCC ignores loops");continue;}
		adj[u-1]|=1LL<<(v-1);adj[v-1]|=1LL<<(u-1);
	}
	int full=(1LL<<n)-1;vector<int> candidates;
	for(int mask=1;mask<=full;mask++){
		if(components(mask,adj)!=1) continue;
		bool ok=true;
		for(int i=0;i<n;i++) if(mask>>i&1){
			if(components(mask^(1LL<<i),adj)>1) ok=false;
			if((mask&(mask-1))==0&&adj[i]) ok=false;
		}
		if(ok) candidates.push_back(mask);
	}
	set<int> expected;
	for(int a:candidates){
		bool maximal=true;
		for(int b:candidates) if(a!=b&&(a&b)==a) maximal=false;
		if(maximal) expected.insert(a);
	}
	for(int repeat=0;repeat<2;repeat++){
		g.build();set<int> actual;
		vector<vector<int>> membership(n+1);
		for(int id=1;id<=g.cnt;id++){
			int mask=0;
			for(int u:g.dcc[id]){
				require(u>=1&&u<=n&&!(mask>>(u-1)&1),"BCC unique members");
				mask|=1LL<<(u-1);membership[u].push_back(id);
			}
			require(actual.insert(mask).second,"BCC unique blocks");
		}
		require(actual==expected,"BCC vs maximal subsets without articulation");
		int base=components(full,adj);
		for(int u=1;u<=n;u++){
			require(g.cut[u]==(components(full^(1LL<<(u-1)),adj)>base),"BCC cut vs vertex removal");
			require(g.belong[u]==membership[u],"BCC multiple memberships");
		}
	}
	g.init(1);g.build();require(g.cnt==1&&g.dcc[1]==vector<int>{1}&&!g.cut[1],"BCC reinit");
}

void check_bcc(){
	vector<pair<int,int>> choices;
	for(int u=1;u<=5;u++) for(int v=u+1;v<=5;v++) choices.push_back({u,v});
	for(int mask=0;mask<(1LL<<10);mask++){
		vector<pair<int,int>> edges;
		for(int i=0;i<10;i++) if(mask>>i&1) edges.push_back(choices[i]);
		verify_bcc(5,edges);
	}
	for(int t=0;t<300;t++){
		int n=rng()%7+1;vector<pair<int,int>> edges;
		for(int i=0,m=rng()%25;i<m;i++) edges.push_back({rng()%n+1,rng()%n+1});
		verify_bcc(n,edges);
	}
}

int trial_mu(int x){
	int res=1;
	for(int p=2;p<=x/p;p++) if(x%p==0){
		x/=p;res=-res;if(x%p==0) return 0;
	}
	if(x>1) res=-res;
	return res;
}

void check_mobius(){
	Sieve sv;
	for(int N:vector<int>{0,1,3000,20,100}){
		sv.init(N);long long prefix=0;vector<int32_t> primes;
		for(int x=1;x<=N;x++){
			int mu=trial_mu(x);prefix+=mu;
			require(sv.mu[x]==mu&&sv.pre_mu[x]==prefix,"Mobius trial factors and prefix");
			int sum=0;for(int d=1;d<=x;d++) if(x%d==0) sum+=sv.mu[d];
			require(sum==(x==1),"Mobius divisor identity");
			if(x>1){
				int p=2;while(x%p) p++;
				require(sv.minp[x]==p,"Sieve minp unchanged");
				if(p==x) primes.push_back(x);
			}
			if(x<=100){
				int phi=0;for(int i=1;i<=x;i++) phi+=gcd(i,x)==1;
				require(sv.phi[x]==phi,"Sieve phi unchanged");
			}
		}
		require(sv.primes==primes,"Sieve primes and reset");
		if(N>=1){
			auto h=sv.prime_xor(123),again=sv.prime_xor(123);
			require(h==again&&h[1]==0,"Sieve seeded hash unchanged");
			for(int x=2;x<=N;x++) if(sv.minp[x]!=x) require(h[x]==(h[x/sv.minp[x]]^h[sv.minp[x]]),"Sieve hash recurrence");
		}
	}
	for(int t=0;t<150;t++){
		int n=rng()%100+1;sv.init(n);
		vector<long long> original(n+1),F(n+1);
		for(int x=1;x<=n;x++) original[x]=(long long)(rng()%20001)-10000;
		for(int x=1;x<=n;x++) for(int d=1;d<=x;d++) if(x%d==0) F[x]+=original[d];
		auto f=mobius_invert(F,sv);
		for(int x=1;x<=n;x++) require(f[x]==original[x],"Mobius inversion recovers independent divisor sums");
	}
	sv.init(100);
	vector<long long> F(101);
	for(int i=1;i<=100;i++) F[i]=i&1?LLONG_MIN:LLONG_MAX;
	auto f=mobius_invert(F,sv);
	for(int x=1;x<=100;x++){
		__int128 sum=0;for(int d=1;d<=x;d++) if(x%d==0) sum+=f[d];
		require(sum==F[x],"Mobius 128-bit inversion");
	}
	require(mobius_invert(vector<long long>{0},sv)==vector<__int128>{0},"Mobius empty effective range");
	for(int n=0;n<=40;n++) for(int m=0;m<=40;m++){
		vector<long long> cnt(41);
		for(int i=1;i<=n;i++) for(int j=1;j<=m;j++) cnt[gcd(i,j)]++;
		require(coprime_pairs(n,m,sv)==cnt[1],"Coprime pairs vs gcd enumeration");
		for(int k=1;k<=10;k++) require(gcd_pairs(n,m,k,sv)==cnt[k],"Exact gcd pairs vs enumeration");
	}
	sv.init(2);
	__int128 n=LLONG_MAX;
	require(coprime_pairs(LLONG_MAX,2,sv)==n+(n+1)/2,"Coprime 128-bit count");
	require(coprime_pairs(0,LLONG_MAX,sv)==0,"Coprime zero dimension");
}

signed main(){
	for(auto [name,fn]:vector<pair<string,void(*)()>>{{"binary Trie",check_trie},
		{"0-1 BFS",check_bfs},{"Euler walks",check_euler},{"point BCC",check_bcc},{"Mobius and Sieve",check_mobius}}){
		long long before=checks;fn();cout<<"PASS: "<<name<<": "<<checks-before<<" checks"<<endl;
	}
	cout<<"PASS: basic additions: "<<checks<<" checks (int size "<<sizeof(int)<<")"<<endl;
}
