// Sources are included by scripts/check_graphs.py; independent subset enumeration below.
long long closure_checks=0,closure_cases=0;
void closure_require(bool ok,const char* msg){
	closure_checks++;
	if(!ok){cerr<<"FAIL: maximum closure: "<<msg<<'\n';exit(1);}
}
void compare_closure(const vector<long long>& w,const vector<pair<int,int>>& edges){
	int n=(int)w.size()-1;__int128 best=0;
	for(int mask=0;mask<(1LL<<n);mask++){
		bool valid=true;
		for(auto [u,v]:edges) if((mask>>(u-1)&1)&&!(mask>>(v-1)&1)){valid=false;break;}
		if(!valid) continue;
		__int128 sum=0;
		for(int u=1;u<=n;u++) if(mask>>(u-1)&1) sum+=w[u];
		best=max(best,sum);
	}
	auto res=max_weight_closure(w,edges);closure_cases++;
	closure_require(res.weight==best,"optimal weight versus all valid subsets");
	vector<bool> chosen(n+1,false);__int128 sum=0;bool valid=true;
	int last=0;
	for(auto u:res.chosen){
		if(u<1||u>n||u<=last){valid=false;break;}
		last=u;chosen[u]=true;sum+=w[u];
	}
	for(auto [u,v]:edges) if(chosen[u]&&!chosen[v]) valid=false;
	closure_require(valid&&sum==best,"recovered unique sorted vertices, original weights and dependencies");
}
signed main(){
	compare_closure({0},{});
	for(int n=1;n<=3;n++){
		int patterns=1;for(int i=0;i<n;i++) patterns*=3;
		for(int code=0;code<patterns;code++){
			vector<long long> w(n+1);int k=code;
			for(int i=1;i<=n;i++){w[i]=vector<long long>{-2,0,3}[k%3];k/=3;}
			for(int mask=0;mask<(1LL<<(n*n));mask++){
				vector<pair<int,int>> edges;
				for(int u=1;u<=n;u++) for(int v=1;v<=n;v++){
					if(mask>>((u-1)*n+v-1)&1) edges.push_back({u,v});
				}
				if(mask%7==0&&!edges.empty()) edges.push_back(edges[0]);
				compare_closure(w,edges);
			}
		}
	}
	mt19937_64 rng(20261015);
	for(int t=0;t<1000;t++){
		int n=4+t%5;vector<long long> w(n+1);vector<pair<int,int>> edges;
		for(int u=1;u<=n;u++){
			w[u]=(long long)(rng()%41)-20;
			for(int v=1;v<=n;v++) if(rng()%5==0) edges.push_back({u,v});
		}
		compare_closure(w,edges);
	}
	compare_closure({0,8,6,-5,-4},{{1,3},{2,3},{2,4}});
	compare_closure({0,8,6,-9},{{1,3},{2,3}});
	compare_closure({0,5,-8,6},{{1,2},{2,1},{3,1}});
	compare_closure({0,7,-2,0,4},{{1,2},{1,3},{1,3},{3,3}});
	compare_closure({0,-1,LLONG_MIN,0},{{1,2},{3,2}});
	compare_closure({0,5,-5},{{1,2}});
	long long p=Dinic::INF-1;
	compare_closure({0,p,LLONG_MIN},{{1,2}});
	compare_closure({0,p,-p+1},{{1,2}});
	compare_closure({0,p,-p},{{1,2}});
	compare_closure({0,2000000000000000000LL,1999999999999999999LL,LLONG_MIN},{{1,3},{2,3}});
	compare_closure({0,2000000000000000000LL,1999999999999999999LL},{{1,2},{2,1}});
	compare_closure({0,1,-1,LLONG_MIN},{{1,2},{2,3}});
	cout<<"PASS: maximum closure, "<<closure_cases<<" graphs / "<<closure_checks<<" independent subset and recovery checks\n";
}
