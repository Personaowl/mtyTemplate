#include<bits/stdc++.h>
#ifdef MTY_INT64
#define int long long
#endif
using namespace std;
#include "code/data-structures/rollback_dsu.cpp"
#include "code/data-structures/time_segment.cpp"

struct Edge{int x,y,l,r;};
long long checks=0,cases=0;
void require(bool ok,const string& message){
	checks++;
	if(!ok){cerr<<"FAIL: "<<message<<'\n';exit(1);}
}
bool color_model(int n,int t,const vector<Edge>& edges){
	vector<vector<int>> e(n+1);
	for(auto [x,y,l,r]:edges) if(l<=t&&t<=r){e[x].push_back(y);e[y].push_back(x);}
	vector<int> color(n+1,-1);
	for(int s=1;s<=n;s++) if(color[s]==-1){
		queue<int> q;q.push(s);color[s]=0;
		while(!q.empty()){
			int u=q.front();q.pop();
			for(auto v:e[u]){
				if(color[v]==-1){color[v]=color[u]^1;q.push(v);}
				else if(color[v]==color[u]) return false;
			}
		}
	}
	return true;
}
void verify(int n,int k,const vector<Edge>& edges){
	cases++;
	RollbackDSU dsu;dsu.init(2*n);
	TimeSegTree seg(k,n);seg.build(1,1,k);
	for(auto [x,y,l,r]:edges) seg.modify(1,l,r,{x,y});
	vector<int> want(k+1);
	for(int t=1;t<=k;t++) want[t]=color_model(n,t,edges);
	auto run=[&](const vector<int>& expected){
		seg.dfs(1,dsu);
		for(int t=1;t<=k;t++) require(seg.ans[t]==expected[t],"time leaf vs independent BFS coloring");
		require(dsu.time()==0,"DFS restores history, including conflict/pruning paths");
		for(int u=1;u<=2*n;u++) require(dsu.fa[u]==u&&dsu.sz[u]==1,"DFS restores parents and component sizes");
	};
	run(want);run(want);
	seg.modify(1,1,k,{1,1});
	run(vector<int>(k+1,0));
	seg.build(1,1,k);
	run(vector<int>(k+1,1));
}
void input_output_checks(){
	auto run=[](const string& input,const string& expected){
		istringstream in(input);ostringstream out;
		auto *oldin=cin.rdbuf(in.rdbuf()),*oldout=cout.rdbuf(out.rdbuf());
		cin.clear();solve_time_bipartite();
		cin.rdbuf(oldin);cout.rdbuf(oldout);cin.clear();
		require(out.str()==expected,"solve adapter, (l,r] boundaries and n!=k");
	};
	run("4 3 6\n1 2 0 4\n2 3 2 6\n3 1 3 5\n","Yes\nYes\nYes\nNo\nYes\nYes\n");
	run("5 2 1\n1 2 0 0\n5 5 0 1\n","No\n");
	run("1 1 3\n1 1 1 2\n","Yes\nNo\nYes\n");
	run("2 0 4\n","Yes\nYes\nYes\nYes\n");
}
signed main(){
	verify(1,1,{});
	verify(1,3,{{1,1,2,2}});
	verify(3,8,{{1,2,1,8},{2,3,1,8},{3,1,3,5}});
	verify(5,1,{{4,5,1,1},{5,5,1,0}});
	verify(2,9,{{1,2,1,9},{1,2,1,9},{2,2,4,7}});
	verify(4,6,{{1,2,1,2},{2,3,3,4},{1,3,5,6},{1,2,3,4},{2,4,5,6}});
	// Enumerate static graphs, including self-loops, disconnected parts and odd cycles.
	for(int n=1;n<=4;n++){
		vector<pair<int,int>> possible;
		for(int x=1;x<=n;x++) for(int y=x;y<=n;y++) possible.push_back({x,y});
		for(int mask=0;mask<(1LL<<possible.size());mask++){
			vector<Edge> edges;
			for(int j=0;j<(int)possible.size();j++) if(mask>>j&1){
				auto [x,y]=possible[j];edges.push_back({x,y,1,3});
			}
			verify(n,3,edges);
		}
	}
	// Enumerate all interval choices of the three edges of a triangle on three times.
	vector<pair<int,int>> spans={{1,0}};
	for(int l=1;l<=3;l++) for(int r=l;r<=3;r++) spans.push_back({l,r});
	for(auto [a,b]:spans) for(auto [c,d]:spans) for(auto [e,f]:spans){
		verify(3,3,{{1,2,a,b},{2,3,c,d},{3,1,e,f}});
	}
	mt19937_64 rng(20261016);
	for(int test=0;test<600;test++){
		int n=rng()%8+1,k=rng()%10+1,m=rng()%25;
		vector<Edge> edges;
		for(int i=0;i<m;i++){
			int x=rng()%n+1,y=rng()%n+1,l=rng()%(k+1),r=rng()%(k+1);
			if(l>r) swap(l,r);
			edges.push_back({x,y,l+1,r});
		}
		verify(n,k,edges);
	}
	input_output_checks();
	cout<<"PASS: time segment, "<<cases<<" graph/interval models, "<<checks<<" checks\n";
}
