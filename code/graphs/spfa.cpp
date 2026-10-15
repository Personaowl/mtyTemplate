struct SPFA{
	static constexpr long long INF=4e18;
	vector<vector<pair<int,long long>>> e;
	vector<long long> dis;
	void init(int n){e.assign(n+1,{});dis.clear();}
	void add(int u,int v,long long w){e[u].push_back({v,w});}
	bool run(int s=0){
		int n=(int)e.size()-1;
		const __int128 BIG=(__int128)1<<120;
		vector<__int128> d(n+1,BIG);
		vector<int> len(n+1,0);
		vector<bool> in(n+1,false);
		queue<int> q;
		dis.clear();
		if(s){d[s]=0;q.push(s);in[s]=true;}
		else for(int u=1;u<=n;u++){
			d[u]=0;q.push(u);in[u]=true;
		}
		while(!q.empty()){
			int u=q.front();q.pop();in[u]=false;
			for(auto [v,w]:e[u]){
				if(d[v]<=d[u]+w) continue;
				d[v]=d[u]+w;
				len[v]=len[u]+1;
				if(len[v]>=n) return false;
				if(!in[v]){q.push(v);in[v]=true;}
			}
		}
		dis.assign(n+1,INF);
		for(int u=1;u<=n;u++) if(d[u]!=BIG) dis[u]=d[u];
		return true;
	}
};
