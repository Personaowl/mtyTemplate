struct Bipartite{
	vector<vector<int>> e;
	vector<int> col;
	void init(int n){e.assign(n+1,{});col.clear();}
	void add(int u,int v){e[u].push_back(v);e[v].push_back(u);}
	bool dfs(int u,int c){
		col[u]=c;
		for(auto v:e[u]){
			if(!col[v]){if(!dfs(v,3-c)) return false;}
			else if(col[v]==c) return false;
		}
		return true;
	}
	bool build(){
		col.assign(e.size(),0);
		for(int u=1;u<(int)e.size();u++) if(!col[u]&&!dfs(u,1)){
			col.clear();return false;
		}
		return true;
	}
};
