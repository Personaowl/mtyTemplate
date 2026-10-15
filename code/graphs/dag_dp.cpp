// Topological order: https://cp-algorithms.com/graph/topological-sort.html
struct DAGDP{
	static constexpr long long INF=4e18;
	int n;
	vector<vector<pair<int,long long>>> e;
	vector<int> in,ord;
	void init(int n){
		this->n=n;e.assign(n+1,{});
		in.assign(n+1,0);ord.clear();
	}
	void add(int u,int v,long long w=1){e[u].push_back({v,w});in[v]++;}
	bool build(){
		auto deg=in;ord.clear();
		queue<int> q;
		for(int u=1;u<=n;u++) if(!deg[u]) q.push(u);
		while(!q.empty()){
			int u=q.front();q.pop();ord.push_back(u);
			for(auto [v,w]:e[u]) if(--deg[v]==0) q.push(v);
		}
		if((int)ord.size()!=n){ord.clear();return false;}
		return true;
	}
	vector<long long> path(int s,bool longest=false)const{
		long long inf=longest?-INF:INF;
		vector<long long> dis(n+1,inf);dis[s]=0;
		for(auto u:ord){
			if(dis[u]==inf) continue;
			for(auto [v,w]:e[u]){
				__int128 nd=(__int128)dis[u]+w;
				if(longest?nd>dis[v]:nd<dis[v]) dis[v]=nd;
			}
		}
		return dis;
	}
	vector<long long> count(int s,long long mod)const{
		vector<long long> cnt(n+1,0);cnt[s]=1%mod;
		for(auto u:ord) for(auto [v,w]:e[u]){
			cnt[v]=((__int128)cnt[v]+cnt[u])%mod;
		}
		return cnt;
	}
};
