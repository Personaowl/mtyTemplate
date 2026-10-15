struct EulerWalk{
	int n;
	bool directed;
	vector<vector<pair<int,int>>> e;
	vector<pair<int,int>> edges;
	vector<int> in,out,path,eid;
	void init(int n,bool directed=false){
		this->n=n;this->directed=directed;
		e.assign(n+1,{});edges.clear();
		in.assign(n+1,0);out.assign(n+1,0);
		path.clear();eid.clear();
	}
	int add(int u,int v){
		int id=edges.size();edges.push_back({u,v});
		e[u].push_back({v,id});out[u]++;in[v]++;
		if(!directed){e[v].push_back({u,id});out[v]++;in[u]++;}
		return id;
	}
	bool build(int start=0){
		path.clear();eid.clear();
		if(n<1||start<0||start>n) return false;
		int s=0,t=0,odd=0;
		for(int u=1;u<=n;u++){
			if(directed){
				int d=out[u]-in[u];
				if(d==1){if(s) return false;s=u;}
				else if(d==-1){if(t) return false;t=u;}
				else if(d!=0) return false;
			}else if(out[u]&1){odd++;if(!s) s=u;}
		}
		if(directed){
			if((s==0)!=(t==0)) return false;
			if(s&&start&&start!=s) return false;
		}else{
			if(odd!=0&&odd!=2) return false;
			if(odd==2&&start&&!(out[start]&1)) return false;
		}
		if(!start) start=s;
		if(!start) for(int u=1;u<=n;u++) if(out[u]){start=u;break;}
		if(!start) start=1;
		int m=edges.size();
		if(m&&!out[start]) return false;
		vector<int> cur(n+1,0);
		vector<bool> used(m,false);
		auto dfs=[&](auto &&self,int u)->void{
			while(cur[u]<(int)e[u].size()){
				auto [v,id]=e[u][cur[u]++];
				if(used[id]) continue;
				used[id]=true;self(self,v);eid.push_back(id);
			}
			path.push_back(u);
		};
		dfs(dfs,start);
		if((int)eid.size()!=m){path.clear();eid.clear();return false;}
		reverse(path.begin(),path.end());reverse(eid.begin(),eid.end());
		return true;
	}
};
