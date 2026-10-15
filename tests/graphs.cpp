// Template sources are included by scripts/check_graphs.py.
long long checks=0;
void require(bool ok,const char *name){
	checks++;
	if(!ok){cerr<<"FAIL: "<<name<<'\n';exit(1);}
}
void fixed_cases(){
	Dijkstra dj;dj.init(4);
	dj.add(1,2,10),dj.add(1,2,2),dj.add(2,3,0),dj.add(1,3,20);
	dj.run(1);
	require(dj.dis[3]==2&&dj.dis[4]==Dijkstra::INF,"Dijkstra parallel, zero, unreachable");
	dj.run(4);require(dj.dis[4]==0&&dj.dis[1]==Dijkstra::INF,"Dijkstra new source");
	SPFA sp;sp.init(4);
	sp.add(1,2,-3),sp.add(2,3,2);
	require(sp.run(1)&&sp.dis[3]==-1&&sp.dis[4]==SPFA::INF,"SPFA negative edge");
	sp.add(4,4,-1);
	require(sp.run(1)&&!sp.run(),"reachable versus global negative cycle");
	sp.init(2);sp.add(1,2,-1),sp.add(2,1,0);
	require(!sp.run(1)&&sp.dis.empty(),"reachable negative cycle");
	Floyd fl;fl.init(3);fl.add(1,2,5),fl.add(1,2,2),fl.add(2,3,-1);
	require(fl.run()&&fl.dis[1][3]==1&&fl.dis[3][1]==Floyd::INF,"Floyd guards and parallel edges");
	fl.init(2);fl.add(1,2,-3),fl.add(2,1,1);
	require(!fl.run(),"Floyd negative cycle");
	Kruskal kr;Prim pr;kr.init(3);pr.init(3);
	for(auto [u,v,w]:vector<tuple<int,int,long long>>{{1,2,-2},{1,2,4},{2,3,3},{1,3,8}}){
		kr.add(u,v,w);pr.add(u,v,w);
	}
	require(kr.run()&&pr.run()&&kr.cost==1&&pr.cost==1,"negative-weight MST");
	kr.init(3);pr.init(3);kr.add(1,2,1);pr.add(1,2,1);
	require(!kr.run()&&!pr.run(),"disconnected MST");
	kr.init(1);pr.init(1);
	require(kr.run()&&pr.run()&&kr.cost==0&&pr.cost==0,"one vertex MST");
	DiscountPath lp;lp.init(3,1);
	lp.add(1,2,8),lp.add(2,3,6),lp.add(1,3,20);lp.run(1);
	require(lp.query(3)==10&&lp.dis[3][0]==14,"layered at most and exact");
	SCC sc;sc.init(5);
	sc.add(1,2),sc.add(2,1),sc.add(2,3),sc.add(3,4),sc.add(4,3),sc.add(5,5);
	require(sc.build()==3&&sc.bel[1]==sc.bel[2]&&sc.bel[3]==sc.bel[4],"SCC partition");
	require(sc.bel[2]>sc.bel[3]&&sc.dag()[sc.bel[2]].size()==1,"SCC reverse topological order");
	require(sc.build()==3,"SCC rebuild");
	Cuts cu;cu.init(5);
	int p0=cu.add(1,2),p1=cu.add(1,2),b=cu.add(2,3);
	cu.add(3,3),cu.add(3,4);
	cu.build();
	require(!cu.bridge[p0]&&!cu.bridge[p1]&&cu.bridge[b],"bridge parallel and self loop");
	require(cu.cut[2]&&cu.cut[3]&&!cu.cut[1]&&!cu.cut[5],"articulation root and disconnected");
	require(cu.bel[1]==cu.bel[2]&&cu.bel[2]!=cu.bel[3]&&cu.cnt==4,"edge components");
	cu.init(3);cu.add(1,2),cu.add(1,3);cu.build();
	require(cu.cut[1],"DFS root with two children");
	TwoSAT sat;sat.init(2);sat.force(1,true);sat.imply(1,true,2,false);
	require(sat.solve()&&sat.ans[1]==1&&sat.ans[2]==0,"2SAT assignment direction");
	sat.force(2,true);require(!sat.solve(),"2SAT contradiction");
	sat.init(2);sat.either(1,false,2,false);
	require(sat.solve()&&!(sat.ans[1]&&sat.ans[2]),"2SAT clause satisfaction");
	DifferenceConstraints dc;dc.init(3);dc.le(1,2,3),dc.ge(2,3,1),dc.eq(1,3,2);
	require(dc.solve()&&dc.val[1]-dc.val[2]<=3&&dc.val[2]-dc.val[3]>=1
		&&dc.val[1]-dc.val[3]==2,"difference constraint directions");
	dc.init(3);dc.le(2,3,-1),dc.le(3,2,-1);
	require(!dc.solve(),"disconnected infeasible constraints");
	MinCycle mc;mc.init(3,false);mc.add(1,2,2),mc.add(2,3,3),mc.add(3,1,4);
	require(mc.run()==9,"undirected triangle");
	mc.init(2,false);mc.add(1,2,2),mc.add(1,2,3),mc.add(1,1,1);
	require(mc.run()==MinCycle::INF,"exclude short cycles");
	mc.init(2);mc.add(1,2,10),mc.add(1,2,2),mc.add(1,2,3);
	require(mc.run()==5,"two distinct parallel edges form cycle");
	mc.add(1,1,1);require(mc.run()==1,"self loop cycle");
	mc.init(4);mc.add(1,2,1),mc.add(2,3,1),mc.add(3,4,1);
	require(mc.run()==MinCycle::INF,"tree has no cycle");
	vector<tuple<int,int,long long>> de={{1,2,2},{2,3,3},{3,1,4}};
	require(directed_min_cycle(3,de)==9,"directed cycle");
	require(directed_min_cycle(2,{{1,2,1}})==Floyd::INF,"directed acyclic");
	require(directed_min_cycle(2,{{1,1,3},{1,2,1},{2,1,1}})==2,"directed two-edge and self cycle");
	Dinic di;di.init(6);
	vector<tuple<int,int,long long>> raw={{1,2,16},{1,3,13},{2,3,10},{3,2,4},{2,4,12},
		{4,3,9},{3,5,14},{5,4,7},{4,6,20},{5,6,4}};
	for(auto [u,v,c]:raw) di.add(u,v,c);
	require(di.flow(1,6)==23,"classic maximum flow");
	auto side=di.reachable(1);long long cut=0;
	for(auto [u,v,c]:raw) if(side[u]&&!side[v]) cut+=c;
	require(cut==23&&!side[6],"minimum cut recovery");
	di.init(3);di.add(1,1,100),di.add(1,2,2),di.add(1,2,3),di.add(2,1,7),di.add(2,3,4);
	require(di.flow(1,3,2)==2&&di.flow(1,3)==2&&di.flow(1,3)==0,"limited and repeated max flow");
	MinCostFlow mf;mf.init(4);
	mf.add(1,2,1,0),mf.add(1,3,1,0),mf.add(2,3,1,-2),mf.add(2,4,1,0),mf.add(3,4,1,0);
	auto res=mf.flow(1,4);
	require(res.ok&&res.flow==2&&res.cost==0,"cost flow reverse-edge rerouting");
	mf.init(3);mf.add(1,2,2,-3),mf.add(2,3,2,5),mf.add(1,3,1,9);
	res=mf.flow(1,3,1);auto more=mf.flow(1,3,3);
	require(res.ok&&res.flow==1&&res.cost==2&&more.ok&&more.flow==2&&more.cost==11,"cost flow limit and continuation");
	mf.init(3);mf.add(1,2,1,2),mf.add(3,3,1,-1);res=mf.flow(1,2);
	require(!res.ok,"initial negative cost cycle rejection");
	mf.init(2);mf.add(1,1,3,0);int eid=mf.add(1,2,2,3);mf.add(1,2,1,1);
	res=mf.flow(1,2);
	require(res.ok&&res.flow==3&&res.cost==7&&mf.e[eid^1].cap==2,"cost flow parallel and self edges");
	mf.init(6);mf.add(5,1,1,0),mf.add(5,2,1,0),mf.add(3,6,1,0),mf.add(4,6,1,0);
	mf.add(1,3,1,-7),mf.add(2,4,1,3),mf.add(5,6,2,0);res=mf.flow(5,6,2);
	require(res.ok&&res.flow==2&&res.cost==-7,"optional maximum-weight matching");
	di.init(5);int a=di.add(1,2,2),c=di.add(2,3,2),d=di.add(3,1,2);
	di.add(4,2,1),di.add(1,5,1);
	require(di.flow(4,5)==1&&di.e[a^1].cap==0&&di.e[c^1].cap==1&&di.e[d^1].cap==1,"lower-bound circulation restoration");
	const int N=100000;
	sc.init(N);cu.init(N);di.init(N);
	for(int u=1;u<N;u++){sc.add(u,u+1);cu.add(u,u+1);di.add(u,u+1,1);}
	require(sc.build()==N&&sc.bel[1]>sc.bel[N],"iterative SCC long chain");
	cu.build();
	int cuts=0,bridges=0;
	for(int u=1;u<=N;u++) cuts+=cu.cut[u];
	for(auto v:cu.bridge) bridges+=v;
	require(cuts==N-2&&bridges==N-1,"iterative cuts long chain");
	require(di.flow(1,N)==1,"iterative Dinic long chain");
}

int components(int n,const vector<pair<int,int>> &edges,int vertex=-1,int edge=-1){
	vector<bool> seen(n+1,false);int count=0;
	for(int s=1;s<=n;s++) if(s!=vertex&&!seen[s]){
		count++;vector<int> st(1,s);seen[s]=true;
		while(!st.empty()){
			int u=st.back();st.pop_back();
			for(int i=0;i<(int)edges.size();i++){
				if(i==edge) continue;
				auto [a,b]=edges[i];
				if(a==vertex||b==vertex) continue;
				int v=a==u?b:(b==u?a:0);
				if(v&&!seen[v]){seen[v]=true;st.push_back(v);}
			}
		}
	}
	return count;
}
void tiny_audit(){
	vector<pair<int,int>> directed,undirected;
	for(int u=1;u<=4;u++) for(int v=1;v<=4;v++) if(u!=v){
		directed.push_back({u,v});
		if(u<v) undirected.push_back({u,v});
	}
	for(int mask=0;mask<(1<<(int)directed.size());mask++){
		SCC g;g.init(4);Dinic f;f.init(4);
		bool reach[5][5]={};
		for(int u=1;u<=4;u++) reach[u][u]=true;
		for(int j=0;j<(int)directed.size();j++) if(mask>>j&1){
			auto [u,v]=directed[j];g.add(u,v);f.add(u,v,1);reach[u][v]=true;
		}
		for(int k=1;k<=4;k++) for(int u=1;u<=4;u++) for(int v=1;v<=4;v++)
			reach[u][v]=reach[u][v]||(reach[u][k]&&reach[k][v]);
		g.build();bool ok=true;
		for(int u=1;u<=4;u++) for(int v=1;v<=4;v++)
			ok&=(g.bel[u]==g.bel[v])==(reach[u][v]&&reach[v][u]);
		require(ok,"tiny SCC versus reachability");
		long long best=LLONG_MAX;
		for(int side=0;side<16;side++) if((side&1)&&!(side&8)){
			long long cost=0;
			for(int j=0;j<(int)directed.size();j++) if(mask>>j&1){
				auto [u,v]=directed[j];cost+=((side>>(u-1)&1)&&!(side>>(v-1)&1));
			}
			best=min(best,cost);
		}
		require(f.flow(1,4)==best,"tiny flow versus enumerated cuts");
	}
	for(int mask=0;mask<(1<<(int)undirected.size());mask++){
		Cuts g;g.init(4);MinCycle cycle;cycle.init(4,false);
		vector<pair<int,int>> edges;
		long long w[5][5];
		for(int u=0;u<5;u++) for(int v=0;v<5;v++) w[u][v]=MinCycle::INF;
		for(int j=0;j<(int)undirected.size();j++) if(mask>>j&1){
			auto [u,v]=undirected[j];g.add(u,v);edges.push_back({u,v});
			w[u][v]=w[v][u]=(u*3+v)%5;cycle.add(u,v,w[u][v]);
		}
		g.build();int count=components(4,edges);bool ok=true;
		for(int u=1;u<=4;u++) ok&=g.cut[u]==(components(4,edges,u)>count);
		for(int j=0;j<(int)edges.size();j++) ok&=g.bridge[j]==(components(4,edges,-1,j)>count);
		require(ok,"tiny cuts versus deleting vertices and edges");
		long long best=MinCycle::INF;
		vector<int> p={1,2,3,4};
		do{
			for(int len=3;len<=4;len++){
				long long sum=0;bool exists=true;
				for(int i=0;i<len;i++){
					long long cost=w[p[i]][p[(i+1)%len]];
					if(cost==MinCycle::INF){exists=false;break;}
					sum+=cost;
				}
				if(exists) best=min(best,sum);
			}
		}while(next_permutation(p.begin(),p.end()));
		require(cycle.run()==best,"tiny minimum cycle versus vertex permutations");
	}
	vector<pair<int,int>> clauses;
	for(int a=0;a<4;a++) for(int b=a;b<4;b++) clauses.push_back({a,b});
	for(int mask=0;mask<(1<<(int)clauses.size());mask++){
		TwoSAT sat;sat.init(2);bool exists=false;
		for(int j=0;j<(int)clauses.size();j++) if(mask>>j&1){
			auto [a,b]=clauses[j];sat.either(a/2+1,a%2,b/2+1,b%2);
		}
		auto valid=[&](int assignment){
			for(int j=0;j<(int)clauses.size();j++) if(mask>>j&1){
				auto [a,b]=clauses[j];
				if(((assignment>>(a/2)&1)!=a%2)&&((assignment>>(b/2)&1)!=b%2)) return false;
			}
			return true;
		};
		for(int assignment=0;assignment<4;assignment++) exists|=valid(assignment);
		bool ok=sat.solve();
		require(ok==exists&&(!ok||valid(sat.ans[1]+2*sat.ans[2])),"tiny 2SAT versus all assignments");
	}
	vector<tuple<int,int,long long>> base={{1,2,0},{1,3,1},{2,3,-4},{2,4,2},{3,4,2}};
	for(int code=0;code<243;code++){
		MinCostFlow g;g.init(4);
		vector<int> cap(5);int value=code;
		for(int i=0;i<5;i++){
			cap[i]=value%3;value/=3;
			auto [u,v,w]=base[i];g.add(u,v,cap[i],w);
		}
		long long limit=code%5==4?MinCostFlow::INF:code%5,bf=-1,bc=0;
		for(int choice=0;choice<243;choice++){
			vector<int> net(5,0);int rest=choice;long long cost=0;bool valid=true;
			for(int i=0;i<5;i++){
				int f=rest%3;rest/=3;
				if(f>cap[i]){valid=false;break;}
				auto [u,v,w]=base[i];net[u]+=f;net[v]-=f;cost+=f*w;
			}
			if(!valid||net[2]||net[3]||net[1]<0||net[1]>limit) continue;
			if(net[1]>bf||(net[1]==bf&&cost<bc)){bf=net[1];bc=cost;}
		}
		auto res=g.flow(1,4,limit);
		require(res.ok&&res.flow==bf&&res.cost==bc,"tiny cost flow versus enumerated feasible edge flows");
	}
}
signed main(signed argc,char **argv){
	fixed_cases();
	cout<<"PASS: graph fixed cases ("<<checks<<")\n";
	if(argc>1&&string(argv[1])=="--audit"){
		long long before=checks;tiny_audit();
		cout<<"PASS: graph independent tiny models ("<<checks-before<<")\n";
	}
}
