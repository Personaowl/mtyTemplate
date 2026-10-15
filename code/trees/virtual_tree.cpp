// Requires TreeLCA. Adjacent-LCA construction follows the user's code.
struct VirtualTree:TreeLCA{
	vector<vector<int>> vt;
	vector<int> nodes;
	vector<bool> key;
	void init(int n){
		TreeLCA::init(n);vt.assign(n+1,{});key.assign(n+1,false);nodes.clear();
	}
	void clear(){for(auto u:nodes){vt[u].clear();key[u]=false;}nodes.clear();}
	void prepare(int rt=1){clear();TreeLCA::build(rt);}
	void build(vector<int> a){
		clear();for(auto u:a) key[u]=true;a.push_back(root);
		auto cmp=[&](int u,int v){return dfn[u]<dfn[v];};
		sort(a.begin(),a.end(),cmp);a.erase(unique(a.begin(),a.end()),a.end());
		nodes=a;
		for(int i=1;i<(int)a.size();i++) nodes.push_back(lca(a[i-1],a[i]));
		sort(nodes.begin(),nodes.end(),cmp);
		nodes.erase(unique(nodes.begin(),nodes.end()),nodes.end());
		for(int i=1;i<(int)nodes.size();i++){
			int p=lca(nodes[i-1],nodes[i]);vt[p].push_back(nodes[i]);
		}
	}
};
