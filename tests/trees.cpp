// Template sources are included by scripts/check_trees.py.
long long checks=0;
void require(bool ok,const char *name){
	checks++;if(!ok){cerr<<"FAIL: "<<name<<'\n';exit(1);}
}
vector<int> expand(const HLD &g,int u,int v,bool edge=false){
	vector<int> a;
	for(auto [l,r,rev]:g.path(u,v,edge)){
		if(rev) for(int i=r;i>=l;i--) a.push_back(g.rk[i]);
		else for(int i=l;i<=r;i++) a.push_back(g.rk[i]);
	}
	return a;
}
void fixed_cases(){
	TreeMetrics m;m.init(1);m.build();
	require(m.diameter==0&&m.path==vector<int>{1}&&m.center==vector<int>{1},"singleton metrics");
	m.init(4);m.add(1,2),m.add(2,3),m.add(3,4);m.build();
	require(m.diameter==3&&m.center==vector<int>({2,3}),"unweighted two centers");
	m.init(4);m.add(1,2,0),m.add(2,3,0),m.add(2,4,0);m.build();
	require(m.diameter==0&&m.center.size()==4,"zero-weight multiple centers");
	m.init(3);m.add(1,2,2),m.add(2,3,10);m.build();
	require(m.diameter==12&&m.center==vector<int>{2},"weighted vertex center differs from continuous midpoint");
	TreeCentroid c;c.init(1);c.build();
	require(c.centroid==vector<int>{1}&&c.balance[1]==0,"singleton centroid");
	c.init(4);c.add(1,2),c.add(2,3),c.add(3,4);c.build(4);
	auto centers=c.centroid;sort(centers.begin(),centers.end());
	require(centers==vector<int>({2,3}),"centroids independent of root");
	TreeLCA l;l.init(4);l.add(1,2,-3),l.add(2,3,5);l.build(vector<int>{2,4});
	require(l.lca(1,3)==2&&l.distance(1,3)==2&&l.lca(1,4)==0
		&&l.distance(1,4)==TreeLCA::INF,"LCA forest, supplied roots and negative edges");
	require(l.jump(1,0)==1&&l.jump(1,1)==2&&l.jump(1,2)==0
		&&l.jump(1,-1)==0&&l.jump(1,LLONG_MAX)==0,"ancestor boundary checks");
	l.build(vector<int>{1,4});require(l.lca(2,3)==2&&l.dep[1]==1,"LCA rebuild and reroot");
	RMQLCA r;r.init(1);r.build();require(r.lca(1,1)==1&&r.ord.size()==1,"Euler singleton");
	HLD h;h.init(5);h.add(1,2),h.add(1,3),h.add(2,4),h.add(2,5);h.build();
	require(expand(h,4,3)==vector<int>({4,2,1,3}),"ordered heavy-chain vertex path");
	require(expand(h,4,3,true)==vector<int>({4,2,3})&&h.path(2,2,true).empty(),"edge mode excludes only LCA");
	auto [lo,hi]=h.subtree(4,true);require(lo>hi,"leaf internal-edge interval is empty");
	h.build(3);require(h.fa[1]==3&&h.lca(4,3)==3,"HLD rebuild with different root");
	LongChain lc;lc.init(9);
	for(auto [u,v]:vector<pair<int,int>>{{1,2},{2,3},{3,4},{1,5},{5,6},{5,7},{5,8},{5,9}}) lc.add(u,v);
	lc.build();require(lc.son[1]==2&&lc.jump(4,3)==1&&lc.jump(4,4)==0,"long child chosen by height");
	require(lc.jump(4,0)==4&&lc.jump(4,-1)==0&&lc.jump(4,LLONG_MAX)==0,"long-chain jump boundaries");
	lc.build(9);require(lc.jump(4,5)==9,"long-chain reroot and ladder crossing");
	TreeDifference d;d.init(3);d.add(1,2),d.add(2,3);d.build();
	d.add_point(1,3,2);d.add_point(2,2,-1);d.add_edge(1,3,4);d.add_edge(2,2,9);
	auto [a,b]=d.collect();
	require(a[1]==2&&a[2]==1&&a[3]==2&&b[1]==0&&b[2]==4&&b[3]==4,"node and edge differences at root and singleton path");
	require(d.collect()==make_pair(a,b),"difference collect is nondestructive");
	d.clear();require(d.collect().first==vector<long long>(4,0),"difference clear keeps tree");
	VirtualTree vt;vt.init(5);vt.add(1,2,3),vt.add(1,3,8),vt.add(2,4,5),vt.add(2,5,2);vt.prepare();
	vt.build(vector<int>{3,4,4});
	require(vt.nodes.size()==3&&vt.key[3]&&vt.key[4]&&!vt.key[1],"virtual tree deduplication and forced root");
	require(virtual_cut(vt,{4,5})==3&&virtual_cut(vt,{3,4})==11
		&&virtual_cut(vt,{2,4,4})==3,"virtual cut common prefix and marked ancestor");
	require(virtual_cut(vt,{})==0&&vt.nodes==vector<int>{1}&&vt.vt[2].empty()
		&&!vt.key[4],"empty virtual query and previous-state cleanup");
	require(virtual_cut(vt,{1})==-1&&virtual_cut(vt,{5})==2,"root key impossible and following query resets mark");
	vt.init(10);vt.add(1,2,2500000000000000000LL);
	vector<int> keys;
	for(int u=3;u<=10;u++){vt.add(2,u,1400000000000000000LL);keys.push_back(u);}
	vt.prepare();require(virtual_cut(vt,keys)==2500000000000000000LL,"virtual DP intermediate sum exceeds signed 64 bit");
	CartesianTree ct;ct.build(vector<long long>{0});require(ct.root==0,"empty Cartesian tree");
	ct.build(vector<long long>{0,2,2,1});require(ct.root==3&&ct.l[3]==1&&ct.r[1]==2,"Cartesian duplicate priority");
	ct.build(vector<long long>{0,-3,-1,-1},false);require(ct.root==2&&ct.r[2]==3,"maximum Cartesian stable ties");
	KruskalTree k;k.init(4);k.add(1,1,-100),k.add(1,2,-5),k.add(1,2,2),k.add(2,3,-2);k.build();
	require(k.bottleneck(1,3)==make_pair(true,-2LL)&&!k.bottleneck(1,4).first
		&&k.bottleneck(1,1)==make_pair(true,0LL),"Kruskal negative edges, parallel edges, self and forest");
	require(k.size(1,-6)==1&&k.size(1,-4)==2&&k.size(1,-2)==3&&k.size(4,99)==1,"Kruskal threshold leaf counts");
	k.add(3,4,8);k.build();require(k.bottleneck(1,4)==make_pair(true,8LL)&&k.size(1,8)==4,"Kruskal rebuild with new edge");
	DSUOnTree ds;ds.init(5,3);ds.add(1,2),ds.add(1,3),ds.add(2,4),ds.add(2,5);
	auto colors=ds.run({0,1,2,1,3,2});
	require(colors[1]==3&&colors[2]==2&&colors[3]==1,"DSU on tree distinct colors");
	colors=ds.run({0,1,1,1,1,1},5);require(colors[5]==1&&colors[2]==1,"DSU on tree reset and reroot");
	const int N=1000;lc.init(N);vt.init(N);
	for(int u=1;u<N;u++){lc.add(u,u+1);vt.add(u,u+1,1);}
	lc.build();vt.prepare();
	require(lc.jump(N,N-1)==1&&virtual_cut(vt,{N})==1,"recursive chain and long-chain ladder");
}

void audit_tree(int n,const vector<pair<int,int>> &edges,int root){
	vector<vector<pair<int,int>>> e(n+1);
	TreeMetrics metric;TreeCentroid centroid;TreeLCA l;RMQLCA rm;HLD h;LongChain lc;
	TreeDifference diff;VirtualTree vt;DSUOnTree ds;
	metric.init(n);centroid.init(n);l.init(n);rm.init(n);h.init(n);lc.init(n);
	diff.init(n);vt.init(n);ds.init(n,3);
	vector<long long> weights;
	for(auto [u,v]:edges){
		int w=(u*7+v*3)%5;weights.push_back(w);
		e[u].push_back({v,w});e[v].push_back({u,w});
		metric.add(u,v,w);centroid.add(u,v);l.add(u,v,w);rm.add(u,v);h.add(u,v);lc.add(u,v);
		diff.add(u,v);vt.add(u,v,w);ds.add(u,v);
	}
	metric.build();centroid.build(root);l.build(root);rm.build(root);h.build(root);
	lc.build(root);diff.build(root);vt.prepare(root);
	vector<int> parent(n+1,0),depth(n+1,0),color(n+1);
	auto dfs=[&](auto &&self,int u,int p)->void{
		parent[u]=p;depth[u]=depth[p]+1;
		for(auto [v,w]:e[u]) if(v!=p) self(self,v,u);
	};
	dfs(dfs,root,0);
	auto ancestor=[&](int u,int v){while(v&&v!=u) v=parent[v];return v==u;};
	auto naive_lca=[&](int u,int v){
		while(depth[u]>depth[v]) u=parent[u];
		while(depth[v]>depth[u]) v=parent[v];
		while(u!=v){u=parent[u];v=parent[v];}return u;
	};
	auto path=[&](int u,int v){
		int p=naive_lca(u,v);vector<int> a,b;
		while(u!=p){a.push_back(u);u=parent[u];}a.push_back(p);
		while(v!=p){b.push_back(v);v=parent[v];}
		reverse(b.begin(),b.end());a.insert(a.end(),b.begin(),b.end());return a;
	};
	vector<vector<long long>> dist(n+1,vector<long long>(n+1,0));
	for(int s=1;s<=n;s++){
		auto walk=[&](auto &&self,int u,int p)->void{
			for(auto [v,w]:e[u]) if(v!=p){dist[s][v]=dist[s][u]+w;self(self,v,u);}
		};walk(walk,s,0);
	}
	long long diameter=0,best=LLONG_MAX;vector<int> center;
	vector<long long> worst(n+1);
	for(int u=1;u<=n;u++){
		worst[u]=*max_element(dist[u].begin()+1,dist[u].end());
		diameter=max(diameter,worst[u]);best=min(best,worst[u]);
		require(max(metric.da[u],metric.db[u])==worst[u],"tiny endpoint distances versus all-pairs distances");
	}
	for(int u=1;u<=n;u++) if(worst[u]==best) center.push_back(u);
	require(metric.diameter==diameter&&metric.center==center,"tiny diameter and centers");
	vector<int> centers;
	for(int removed=1;removed<=n;removed++){
		vector<bool> seen(n+1,false);seen[removed]=true;int biggest=0;
		auto flood=[&](auto &&self,int u)->int{
			seen[u]=true;int count=1;
			for(auto [v,w]:e[u]) if(!seen[v]) count+=self(self,v);
			return count;
		};
		for(int u=1;u<=n;u++) if(!seen[u]) biggest=max(biggest,flood(flood,u));
		require(centroid.balance[removed]==biggest,"tiny centroid balance versus vertex deletion");
		if(biggest<=n/2) centers.push_back(removed);
	}
	auto actual=centroid.centroid;sort(actual.begin(),actual.end());
	require(actual==centers,"tiny centroid set");
	vector<long long> pa(n+1,0),ea(n+1,0);
	for(int u=1;u<=n;u++){
		for(int k=-1;k<=n+1;k++){
			int p=u;
			if(k<0) p=0;else for(int i=0;i<k&&p;i++) p=parent[p];
			require(l.jump(u,k)==p&&lc.jump(u,k)==p,"tiny ancestor versus repeated parent");
		}
		for(int v=1;v<=n;v++){
			int p=naive_lca(u,v);
			require(l.lca(u,v)==p&&rm.lca(u,v)==p&&h.lca(u,v)==p
				&&l.distance(u,v)==dist[u][v]&&l.ancestor(u,v)==ancestor(u,v),"tiny LCA, distance and ancestry");
			auto a=path(u,v);vector<int> b;
			for(int i=1;i<(int)a.size();i++) b.push_back(parent[a[i-1]]==a[i]?a[i-1]:a[i]);
			require(expand(h,u,v)==a&&expand(h,u,v,true)==b,"tiny ordered HLD node and edge paths");
			if(u<=v){
				long long w=(u+v)%5-2;diff.add_point(u,v,w);diff.add_edge(u,v,w);
				for(auto x:a) pa[x]+=w;
				for(auto x:b) ea[x]+=w;
			}
		}
		for(bool edge:{false,true}){
			auto [lo,hi]=h.subtree(u,edge);set<int> got,wanted;
			for(int i=lo;i<=hi;i++) got.insert(h.rk[i]);
			for(int v=1;v<=n;v++) if(ancestor(u,v)&&(!edge||v!=u)) wanted.insert(v);
			require(got==wanted,"tiny HLD subtree intervals");
		}
		color[u]=u%3+1;
	}
	auto [gotp,gote]=diff.collect();bool same=true;
	for(int u=1;u<=n;u++) same&=gotp[u]==pa[u]&&gote[u]==ea[u];
	require(same,"tiny point and edge differences versus path enumeration");
	auto colors=ds.run(color,root);
	for(int u=1;u<=n;u++){
		set<int> s;for(int v=1;v<=n;v++) if(ancestor(u,v)) s.insert(color[v]);
		require(colors[u]==(int)s.size(),"tiny DSU on tree versus explicit subtree colors");
	}
	for(int mask=0;mask<(1<<n);mask++){
		vector<int> keys;set<int> kept{root};
		for(int u=1;u<=n;u++) if(mask>>(u-1)&1){keys.push_back(u);kept.insert(u);}
		for(auto u:keys) for(auto v:keys) kept.insert(naive_lca(u,v));
		if(!keys.empty()) keys.push_back(keys[0]);
		vt.build(keys);set<pair<int,int>> expected,actual_edges;
		for(auto u:kept) if(u!=root){
			int p=parent[u];while(!kept.count(p)) p=parent[p];expected.insert({p,u});
		}
		for(auto u:vt.nodes) for(auto v:vt.vt[u]) actual_edges.insert({u,v});
		require(set<int>(vt.nodes.begin(),vt.nodes.end())==kept&&actual_edges==expected,
			"tiny virtual closure and nearest selected ancestors");
		long long best=LLONG_MAX;
		if(mask>>(root-1)&1) best=-1;
		else for(int cut=0;cut<(1<<(int)edges.size());cut++){
			vector<bool> seen(n+1,false);long long cost=0;
			for(int i=0;i<(int)edges.size();i++) if(cut>>i&1) cost+=weights[i];
			auto visit=[&](auto &&self,int u)->void{
				seen[u]=true;
				for(int i=0;i<(int)edges.size();i++) if(!(cut>>i&1)){
					auto [a,b]=edges[i];int v=a==u?b:(b==u?a:0);
					if(v&&!seen[v]) self(self,v);
				}
			};visit(visit,root);
			bool ok=true;for(auto u:keys) ok&=!seen[u];if(ok) best=min(best,cost);
		}
		require(virtual_cut(vt,keys)==best,"tiny virtual cut versus enumerated deleted-edge sets");
	}
}
void tiny_audit(){
	for(int n=1;n<=5;n++){
		int count=1;for(int i=0;i<n-2;i++) count*=n;
		for(int code=0;code<count;code++){
			vector<int> degree(n+1,1),p;int value=code;
			for(int i=0;i<n-2;i++){p.push_back(value%n+1);degree[p.back()]++;value/=n;}
			vector<pair<int,int>> edges;
			for(auto v:p){
				int u=1;while(degree[u]!=1) u++;
				edges.push_back({u,v});degree[u]--;degree[v]--;
			}
			vector<int> last;for(int u=1;u<=n;u++) if(degree[u]==1) last.push_back(u);
			if(n>1) edges.push_back({last[0],last[1]});
			for(int root=1;root<=n;root++) audit_tree(n,edges,root);
		}
	}
	for(int n=0;n<=6;n++){
		int count=1;for(int i=0;i<n;i++) count*=3;
		for(int code=0;code<count;code++) for(bool small:{false,true}){
			vector<long long> a(n+1,0);int value=code;
			for(int u=1;u<=n;u++){a[u]=value%3-1;value/=3;}
			CartesianTree g;g.build(a,small);vector<int> l(n+1),r(n+1),fa(n+1);
			auto build=[&](auto &&self,int lo,int hi,int parent)->int{
				if(lo>hi) return 0;
				int p=lo;for(int i=lo+1;i<=hi;i++) if(small?a[i]<a[p]:a[i]>a[p]) p=i;
				fa[p]=parent;l[p]=self(self,lo,p-1,p);r[p]=self(self,p+1,hi,p);return p;
			};
			int root=build(build,1,n,0);
			require(g.root==root&&g.l==l&&g.r==r&&g.fa==fa,"tiny Cartesian versus recursive interval extrema");
		}
	}
	vector<pair<int,int>> all;
	for(int u=1;u<=4;u++) for(int v=u+1;v<=4;v++) all.push_back({u,v});
	for(int mask=0;mask<64;mask++){
		KruskalTree g;g.init(4);vector<tuple<int,int,long long>> edges;
		long long d[5][5];
		for(int u=1;u<=4;u++) for(int v=1;v<=4;v++) d[u][v]=u==v?-TreeLCA::INF:TreeLCA::INF;
		for(int j=0;j<6;j++) if(mask>>j&1){
			auto [u,v]=all[j];long long w=j-2;edges.push_back({u,v,w});g.add(u,v,w);d[u][v]=d[v][u]=w;
		}
		g.build();
		for(int k=1;k<=4;k++) for(int u=1;u<=4;u++) for(int v=1;v<=4;v++) d[u][v]=min(d[u][v],max(d[u][k],d[k][v]));
		for(int u=1;u<=4;u++) for(int v=1;v<=4;v++) if(u!=v){
			auto [ok,w]=g.bottleneck(u,v);
			require(ok==(d[u][v]!=TreeLCA::INF)&&(!ok||w==d[u][v]),"tiny Kruskal bottleneck versus min-max Floyd");
		}
		for(int u=1;u<=4;u++) for(int x=-3;x<=4;x++){
			vector<bool> seen(5,false);
			auto visit=[&](auto &&self,int a)->void{
				seen[a]=true;
				for(auto [b,c,w]:edges) if(w<=x){
					int v=b==a?c:(c==a?b:0);if(v&&!seen[v]) self(self,v);
				}
			};visit(visit,u);
			require(g.size(u,x)==count(seen.begin(),seen.end(),true),"tiny threshold component versus edge-filtered reachability");
		}
	}
}
signed main(signed argc,char **argv){
	fixed_cases();cout<<"PASS: tree fixed cases ("<<checks<<")\n";
	if(argc>1&&string(argv[1])=="--audit"){
		long long before=checks;tiny_audit();
		cout<<"PASS: tree independent tiny models ("<<checks-before<<")\n";
	}
}
