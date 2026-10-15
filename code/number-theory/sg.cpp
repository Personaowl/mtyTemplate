struct SG{
	vector<vector<int>> e;
	vector<int> sg;
	void init(int n){
		e.assign(n+1,{});
		sg.assign(n+1,0);
	}
	void add(int u,int v){e[u].push_back(v);}
	bool build(){
		int n=e.size();
		vector<int> deg(n,0),ord;
		for(int u=0;u<n;u++) for(auto v:e[u]) deg[v]++;
		queue<int> q;
		for(int u=0;u<n;u++) if(!deg[u]) q.push(u);
		while(!q.empty()){
			int u=q.front();q.pop();
			ord.push_back(u);
			for(auto v:e[u]) if(!--deg[v]) q.push(v);
		}
		sg.assign(n,0);
		if((int)ord.size()!=n) return false;
		for(int i=n-1;i>=0;i--){
			int u=ord[i],d=e[u].size();
			vector<bool> vis(d+1,false);
			for(auto v:e[u]) if(sg[v]<=d) vis[sg[v]]=true;
			while(sg[u]<=d&&vis[sg[u]]) sg[u]++;
		}
		return true;
	}
};
