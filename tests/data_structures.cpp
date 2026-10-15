#include<bits/stdc++.h>
#ifdef MTY_INT64
#define int long long
#endif
using namespace std;
#include "code/data-structures/dsu.cpp"
#include "code/data-structures/weighted_dsu.cpp"
#include "code/data-structures/kind_dsu.cpp"
#include "code/data-structures/binary_dsu.cpp"
#include "code/data-structures/monotonic_stack.cpp"
#include "code/data-structures/monotonic_queue.cpp"
#include "code/data-structures/median_heap.cpp"
#include "code/data-structures/rollback_dsu.cpp"
#include "code/data-structures/st.cpp"
#include "code/data-structures/bit.cpp"
#include "code/data-structures/bit_distinct.cpp"
#include "code/data-structures/bit_2d_count.cpp"
#include "code/data-structures/odt.cpp"
#include "code/data-structures/seg_point.cpp"
#include "code/data-structures/seg_lazy.cpp"
#include "code/data-structures/seg_affine.cpp"
#include "code/data-structures/seg_max_subarray.cpp"
#include "code/data-structures/seg_equal_run.cpp"
#include "code/data-structures/seg_dynamic.cpp"
#include "code/data-structures/li_chao.cpp"
#include "code/data-structures/persistent_array.cpp"
#include "code/data-structures/persistent_kth.cpp"
#include "code/data-structures/sqrt_block.cpp"
#include "code/data-structures/mo.cpp"
#include "code/data-structures/mo_update.cpp"
#include "code/trees/hld.cpp"
#include "code/data-structures/hld_segment.cpp"

long long checks=0;
void require(bool ok,const string& msg){
	checks++;
	if(!ok){cerr<<"FAIL: "<<msg<<endl;exit(1);}
}
long long sum(const vector<long long>& a,int l,int r){
	return accumulate(a.begin()+l,a.begin()+r+1,0LL);
}
void check_ranges(int n){
	vector<long long> a(n+1);
	for(int i=1;i<=n;i++) a[i]=(i*7)%11-7;
	ST st;st.init(a);
	BIT bit;bit.init(n);
	PointSegTree point(n);SegTree seg(n);
	MaxSubarraySegTree mx(n);EqualRunSegTree eq(n);
	SqrtBlock sq;sq.init(a);
	DynamicSegTree dy;dy.init(1,n);
	point.w=seg.w=mx.w=eq.w=a;
	point.build(1,1,n);seg.build(1,1,n);
	mx.build(1,1,n);eq.build(1,1,n);
	for(int i=1;i<=n;i++){
		bit.add(i,a[i]);dy.modify(i,i,a[i]);
	}
	auto ap=a,ae=a;
	for(int l=1;l<=n;l++) for(int r=l;r<=n;r++){
		require(st.query(l,r)==*max_element(a.begin()+l,a.begin()+r+1),"ST");
	}
	for(int step=0;step<22;step++){
		int l=step*7%n+1,r=step*13%n+1;if(l>r) swap(l,r);
		if(step%5==0){l=1;r=n;}
		long long val=step%9-4;
		seg.modify(1,l,r,val);sq.modify(l,r,val);
		for(int i=l;i<=r;i++){a[i]+=val;bit.add(i,val);}
		point.modify(1,l,val);mx.modify(1,l,val);ap[l]=val;
		eq.modify(1,l,r,val);dy.modify(l,r,val);
		for(int i=l;i<=r;i++) ae[i]=val;
		auto nodes=dy.tr.size();
		for(int L=1;L<=n;L++) for(int R=L;R<=n;R++){
			long long total=sum(a,L,R);
			require(seg.query(1,L,R).sum==total,"lazy add/sum");
			require(bit.sum(L,R)==total,"BIT");
			require(point.query(1,L,R).sum==sum(ap,L,R),"point assign");
			long long best=LLONG_MIN;
			for(int i=L;i<=R;i++) for(int j=i;j<=R;j++) best=max(best,sum(ap,i,j));
			require(mx.query(1,L,R).maxn==best,"nonempty max subarray");
			int run=0,longest=0;
			for(int i=L;i<=R;i++){
				run=(i>L&&ae[i]==ae[i-1])?run+1:1;longest=max(longest,run);
			}
			require(eq.query(1,L,R).maxn==longest,"equal run assignment");
			require(dy.query(L,R)==sum(ae,L,R),"dynamic assignment");
			vector<long long> b(a.begin()+L,a.begin()+R+1);sort(b.begin(),b.end());
			for(int k=1;k<=(int)b.size();k++){
				require(sq.query(L,R,k)==b[k-1],"sqrt kth");
			}
			for(long long x:{-8LL,0LL,7LL}){
				int cnt=count_if(b.begin(),b.end(),[&](long long y){return y<=x;});
				require(sq.count(L,R,x)==cnt,"sqrt count");
			}
		}
		require(dy.tr.size()==nodes,"dynamic query allocates no nodes");
	}
	// Rebuilding resets lazy tags instead of preserving updates.
	seg.build(1,1,n);eq.build(1,1,n);
	st.init(vector<long long>{0,-9});
	bit.init(1);sq.init(vector<long long>{0,-4});dy.init(-3,-3);
	bit.add(1,2);dy.modify(-3,-3,-7);
	for(int k=1;k<=2;k++) require(bit.kth(k)==1,"BIT kth singleton");
	bit.init(n);
	vector<int> freq(n+1);
	for(int i=1;i<=n;i++){freq[i]=i%3;bit.add(i,freq[i]);}
	long long k=0;
	for(int i=1;i<=n;i++) for(int j=0;j<freq[i];j++){
		require(bit.kth(++k)==i,"BIT kth zero gaps");
	}
	if(k) require(bit.kth(k+1)==n+1,"BIT missing rank sentinel");
	require(st.query(1,1)==-9,"reinitialization");
	require(sq.query(1,1,1)==-4&&dy.query(-3,-3)==-7,"signed singleton");
}
void check_affine(){
	for(long long P:{1LL,2LL,1000000007LL,LLONG_MAX}){
		int n=7;AffineSegTree seg(n,P);
		seg.w={0,-3,2,7,-9,4,1,6};seg.build(1,1,n);
		auto a=seg.w;
		auto norm=[&](__int128 x)->long long{x%=P;if(x<0) x+=P;return x;};
		for(auto& x:a) x=norm(x);
		for(int step=0;step<30;step++){
			int l=step*3%n+1,r=step*5%n+1;if(l>r) swap(l,r);
			if(step%4==0){l=1;r=n;}
			long long mul=step%5-2,add=step%7-3;
			if(step==5){mul=LLONG_MAX-2;add=LLONG_MIN;}
			seg.modify(1,l,r,mul,add);
			for(int i=l;i<=r;i++) a[i]=norm((__int128)a[i]*mul+add);
			for(int L=1;L<=n;L++) for(int R=L;R<=n;R++){
				__int128 s=0;for(int i=L;i<=R;i++) s+=a[i];
				require(seg.query(1,L,R).sum==norm(s),"affine chronological composition");
			}
		}
	}
}
void check_versions(){
	for(int n:{(int)1,(int)6,(int)17}){
		vector<long long> a(n+1);
		for(int i=1;i<=n;i++) a[i]=i%4-2;
		PersistentArray pa;pa.init(a);
		vector<vector<long long>> versions{a};
		for(int i=0;i<25;i++){
			int ver=i%(int)versions.size(),p=(i*7)%n+1;
			auto next=versions[ver];int id;
			if(i%4==0) id=pa.copy(ver);
			else{next[p]=i-13;id=pa.modify(ver,p,next[p]);}
			versions.push_back(next);
			require(id==(int)versions.size()-1,"version numbering");
			for(int v=0;v<(int)versions.size();v++) for(int j=1;j<=n;j++){
				require(pa.query(v,j)==versions[v][j],"persistent branching isolation");
			}
		}
		pa.init(vector<long long>{0,8});
		require(pa.root.size()==1&&pa.query(0,1)==8,"persistent array reset");
		PersistentSegTree pk;pk.init(a);
		for(int l=1;l<=n;l++) for(int r=l;r<=n;r++){
			vector<long long> b(a.begin()+l,a.begin()+r+1);sort(b.begin(),b.end());
			for(int k=1;k<=(int)b.size();k++) require(pk.query(l,r,k)==b[k-1],"prefix kth duplicates");
		}
		pk.init(vector<long long>{0,100,100});
		require(pk.query(1,2,2)==100,"prefix tree reset/single value");
	}
}
void check_li_chao(){
	LiChaoTree tree;tree.init(-7,9);
	struct Line{long long l,r,k,b;};
	vector<Line> lines={{-7,9,2,3},{-7,9,2,-2},{-3,5,-4,8},
		{2,2,0,-90},{-7,-7,7,-12},{-7,9,0,0},{-7,9,-2,-10},
		{-7,9,-2,-10},{-6,8,5,-7},{9,9,1,-100}};
	vector<long long> best(17,tree.INF);
	require(tree.query(0)==tree.INF,"empty Li Chao");
	for(auto [l,r,k,b]:lines){
		if(l==-7&&r==9) tree.add(k,b);else tree.add(l,r,k,b);
		for(int x=-7;x<=9;x++){
			if(l<=x&&x<=r) best[x+7]=min(best[x+7],k*x+b);
			require(tree.query(x)==best[x+7],"Li Chao segment/full insertion");
		}
	}
	tree.init(-1000000000,1000000000);
	tree.add(1000000000,0);tree.add(-1000000000,1);
	require(tree.query(-1000000000)==-1000000000000000000LL,"Li Chao wide negative domain");
	tree.init(-1,1);tree.add(0,0,0,-3);
	require(tree.query(1)==tree.INF&&tree.query(0)==-3,"Li Chao reset/uncovered");
	DynamicSegTree seg;seg.init(1,1000000000);
	seg.modify(1,1000000000,-2);seg.modify(2,999999999,0);
	require(seg.query(1,1000000000)==-4,"dynamic huge domain overwrite zero");
}
void check_dsu(){
	DSU plain;plain.init(5);plain.merge(1,2);plain.init(5);
	require(!plain.check(1,2)&&plain.size(1)==1,"shared DSU reset");
	RollbackDSU d;d.init(7);int t=d.time();
	d.merge(1,2);d.merge(3,4);int mid=d.time();
	d.merge(2,4);d.merge(5,6);d.merge(1,3);
	require(d.size(4)==4&&d.time()==4,"successful merge history only");
	d.rollback(mid);
	require(d.check(1,2)&&d.check(3,4)&&!d.check(1,3),"rollback nested snapshot");
	d.merge(4,2);d.revert();d.rollback(t);
	for(int i=1;i<=7;i++) require(d.size(i)==1,"rollback size restoration");
	d.merge(1,2);d.init(3);
	require(d.time()==0&&!d.check(1,2),"rollback reset history");
}
void check_dsu_relations(){
	// The independent weighted model only stores accepted equations as graph edges.
	int n=6;WeightedDSU d;d.init(n);
	vector<vector<pair<int,long long>>> e(n+1);
	auto model=[&](int x,int y)->pair<bool,long long>{
		vector<int> vis(n+1);vector<long long> val(n+1);queue<int> q;
		q.push(x);vis[x]=1;
		while(!q.empty()){
			int u=q.front();q.pop();
			for(auto [v,w]:e[u]) if(!vis[v]){
				vis[v]=1;val[v]=val[u]+w;q.push(v);
			}
		}
		return {vis[y]!=0,val[y]};
	};
	vector<tuple<int,int,long long>> ops={{2,3,-4},{1,2,5},{4,5,7},
		{6,4,-2},{5,1,-3},{3,6,4},{3,6,-3},{1,1,0},{2,2,1},
		{2,1,-5},{5,4,-7},{4,3,2},{6,2,9}};
	for(auto [x,y,w]:ops){
		auto [known,val]=model(x,y);bool ok=!known||val==w;
		require(d.merge(x,y,w)==ok,"weighted equation acceptance");
		if(ok){e[x].push_back({y,w});e[y].push_back({x,-w});}
		for(int u=1;u<=n;u++){
			int cnt=0;
			for(int v=1;v<=n;v++){
				auto expected=model(u,v),actual=d.query(u,v);
				require(actual==expected,"weighted direction/compression/rejected equation");
				require(d.check(u,v)==expected.first,"weighted connectivity");
				cnt+=expected.first;
			}
			require(d.size(u)==cnt,"weighted component size");
		}
	}
	d.init(3);
	require(!d.check(1,2)&&d.size(1)==1,"weighted reset");
	long long big=9000000000000000000LL;
	require(d.merge(1,2,big)&&d.merge(2,3,-big),"weighted wide arithmetic");
	require(d.query(1,3)==make_pair(true,0LL),"weighted cancellation");
	require(d.query(2,1)==make_pair(true,-big),"weighted reversed difference");
	// Exhaust all assignments; no DSU or potential arithmetic in this model.
	for(int k:{(int)2,(int)3,(int)4}){
		int n=4,total=1;for(int i=1;i<=n;i++) total*=k;
		vector<vector<int>> choices;
		for(int mask=0;mask<total;mask++){
			vector<int> c(n+1);int x=mask;
			for(int i=1;i<=n;i++){c[i]=x%k;x/=k;}
			choices.push_back(c);
		}
		KindDSU kind;kind.init(n,k);
		BinaryDSU binary;binary.init(n);
		vector<tuple<int,int,int>> ops={{1,2,1},{3,4,2},{1,3,-1},
			{4,2,0},{2,1,-1},{2,2,1},{2,2,0},{1,4,9},{4,1,-9},
			{3,2,-5},{1,2,0},{3,4,-2}};
		for(auto [x,y,r]:ops){
			auto next=choices;int rel=(r%k+k)%k;
			next.erase(remove_if(next.begin(),next.end(),[&](const vector<int>& c){
				return (c[y]-c[x]+k)%k!=rel;
			}),next.end());
			bool ok=!next.empty();
			require(kind.merge(x,y,r)==ok,"expanded kind constraint acceptance");
			if(k==2) require(binary.merge(x,y,rel)==ok,"binary constraint acceptance");
			if(ok) choices=next;
			for(int u=1;u<=n;u++) for(int v=1;v<=n;v++){
				set<int> vals;
				for(const auto& c:choices) vals.insert((c[v]-c[u]+k)%k);
				auto [known,diff]=kind.query(u,v);
				require(known==(vals.size()==1),"kind known relation");
				if(known) require(diff==*vals.begin(),"kind cyclic direction");
				require(kind.check(u,v)==known,"kind check semantics");
				if(k==2){
					auto [found,r]=binary.query(u,v);
					require(found==known&&(!known||r==diff),"binary exhaustive relation");
					require(binary.same(u,v)==(known&&diff==0),"binary same predicate");
					require(binary.different(u,v)==(known&&diff==1),"binary opposite predicate");
				}
			}
		}
		kind.init(2);
		binary.init(2);
		require(!binary.same(1,2)&&!binary.different(1,2),"binary reset unknown");
		require(binary.merge(1,1,0)&&!binary.merge(1,1,1),"binary self relation");
		require(binary.merge(1,2,1)&&binary.merge(1,2,1),"binary repeated relation");
		require(!binary.merge(1,2,0)&&binary.different(1,2),"binary rejected relation");
		require(!kind.check(1,2)&&kind.merge(1,2,1),"kind reset/default k");
		require(!kind.merge(1,2,0),"kind rejected relation preserves state");
	}
}
int distinct(const vector<int>& a,int l,int r){
	return set<int>(a.begin()+l,a.begin()+r+1).size();
}
void check_mo(){
	for(int n:{(int)1,(int)9,(int)27}){
		vector<int> a(n+1);for(int i=1;i<=n;i++) a[i]=i%6+1;
		Mo mo;MoWithUpdate mt;mo.init(a,6);mt.init(a,6);
		require(mo.work().empty()&&mt.work().empty(),"empty queries");
		vector<int> expected,original;auto w=a;
		for(int step=0;step<55;step++){
			int p=step*7%n+1,val=step%6+1;
			if(step%3){mt.modify(p,val);w[p]=val;}
			int l=step*11%n+1,r=step*5%n+1;if(l>r) swap(l,r);
			mt.add_query(l,r);expected.push_back(distinct(w,l,r));
			mo.add_query(l,r);original.push_back(distinct(a,l,r));
		}
		require(mt.work()==expected&&mt.work()==expected,"Mo time rewinds and repeat calls");
		require(mo.work()==original&&mo.work()==original,"ordinary Mo sorted windows");
		mo.init(vector<int>{0,1},1);mt.init(vector<int>{0,1},1);
		mo.add_query(1,1);mt.modify(1,1);mt.add_query(1,1);
		require(mo.work()==vector<int>{1}&&mt.work()==vector<int>{1},"Mo reset");
	}
}

void check_hld(){
	for(int n:{(int)1,(int)9}) for(int root=1;root<=n;root+=4){
		HLD g;g.init(n);
		for(int i=2;i<=n;i++) g.add(i,i/2);
		g.build(root);
		vector<int> fa(n+1),dep(n+1);queue<int> q;q.push(root);dep[root]=1;
		while(!q.empty()){
			int u=q.front();q.pop();
			for(int v:g.e[u]) if(!dep[v]){fa[v]=u;dep[v]=dep[u]+1;q.push(v);}
		}
		auto path=[&](int x,int y){
			vector<int> res;
			while(x!=y){
				if(dep[x]>=dep[y]){res.push_back(x);x=fa[x];}
				else{res.push_back(y);y=fa[y];}
			}
			res.push_back(x);return res;
		};
		auto inside=[&](int u,int v){while(v&&v!=u) v=fa[v];return v==u;};
		for(long long P:{1LL,7LL,1000000007LL,LLONG_MAX}){
			auto norm=[&](__int128 x)->long long{x%=P;if(x<0) x+=P;return x;};
			vector<long long> a(n+1);ModSegTree seg(n,P);
			for(int i=1;i<=n;i++){a[i]=norm(i*7-20);seg.w[g.dfn[i]]=a[i];}
			seg.build(1,1,n);
			for(int step=0;step<15;step++){
				int x=step*7%n+1,y=step*5%n+1;
				long long z=step==5?LLONG_MIN:step-4;
				if(step%2){
					path_modify(g,seg,x,y,z);
					for(int u:path(x,y)) a[u]=norm((__int128)a[u]+z);
				}
				else{
					seg.modify(1,g.dfn[x],g.dfn[x]+g.sz[x]-1,z);
					for(int u=1;u<=n;u++) if(inside(x,u)) a[u]=norm((__int128)a[u]+z);
				}
				for(int u=1;u<=n;u++){
					for(int v=1;v<=n;v++){
						__int128 res=0;for(int z:path(u,v)) res+=a[z];
						require(path_query(g,seg,u,v)==norm(res),"modular HLD path/reroot");
					}
					__int128 res=0;for(int v=1;v<=n;v++) if(inside(u,v)) res+=a[v];
					require(seg.query(1,g.dfn[u],g.dfn[u]+g.sz[u]-1).sum==norm(res),
						"modular HLD subtree/reroot");
				}
			}
		}
	}
}
void check_odt(){
	for(int n:{(int)1,(int)2,(int)7,(int)17}){
		vector<long long> a(n+1);for(int i=1;i<=n;i++) a[i]=i%3-1;
		ODT odt;odt.init(a);
		for(int step=0;step<25;step++){
			int l=step*7%n+1,r=step*13%n+1;if(l>r) swap(l,r);
			if(step%5==0){l=1;r=n;}
			long long v=step%11-4;
			if(step%2){odt.add(l,r,v);for(int i=l;i<=r;i++) a[i]+=v;}
			else{odt.assign(l,r,v);for(int i=l;i<=r;i++) a[i]=v;}
			for(int L=1;L<=n;L++) for(int R=L;R<=n;R++){
				require(odt.query(L,R)==sum(a,L,R),"ODT split/assign/add/sum");
			}
			require(odt.ans==sum(a,1,n),"ODT global sum");
			int next=1;
			for(auto [l,r,v]:odt.tr){
				require(l==next&&l<=r,"ODT nonempty disjoint coverage");next=r+1;
			}
			require(next==n+1,"ODT full coverage");
			require(odt.split(n+1)==odt.tr.end(),"ODT right sentinel");
		}
		odt.init(1,7);require(odt.ans==7&&odt.query(1,1)==7,"ODT value is not bool");
		odt.assign(1,1,0);require(odt.ans==0,"ODT zero assignment/reset");
	}
}
void check_bit_applications(){
	for(int n:{(int)1,(int)5,(int)17}){
		vector<int> a(n+1);for(int i=1;i<=n;i++) a[i]=i*i%5+1;
		vector<pair<int,int>> ask;
		for(int r=n;r>=1;r--) for(int l=r;l>=1;l--) ask.push_back({l,r});
		auto ans=count_distinct(a,ask);
		for(int i=0;i<(int)ask.size();i++){
			auto [l,r]=ask[i];require(ans[i]==distinct(a,l,r),"last occurrence offline count");
		}
		require(count_distinct(a,{}).empty(),"distinct no queries");
	}
	require(count_distinct({0,1000000,1000000},{{1,2}})==vector<int>{1},"distinct value limit");
	vector<int> coords={0,1,2,3,4,10000000};vector<array<int,4>> ask;
	for(int a:coords) for(int c:coords) if(a<=c){
		for(int b:coords) for(int d:coords) if(b<=d) ask.push_back({a,b,c,d});
	}
	vector<vector<pair<int,int>>> groups={{},{{0,0}},{{2,3},{2,3},{3,2},{0,0}},
		{{10000000,0},{0,10000000},{10000000,10000000},{1,1}}};
	for(const auto& p:groups){
		auto before=p;auto ans=count_points(p,ask);
		for(int i=0;i<(int)ask.size();i++){
			auto [a,b,c,d]=ask[i];long long expected=0;
			for(auto [x,y]:p) expected+=a<=x&&x<=c&&b<=y&&y<=d;
			require(ans[i]==expected,"closed rectangle/repeated points/zero boundary");
		}
		require(count_points(p,{}).empty()&&p==before,"point count no queries/input unchanged");
	}
	auto run=[&](const string& data,const string& expected,auto solve){
		istringstream in(data);ostringstream out;
		auto old_in=cin.rdbuf(in.rdbuf());auto old_out=cout.rdbuf(out.rdbuf());
		cin.clear();solve();cin.rdbuf(old_in);cout.rdbuf(old_out);cin.clear();
		require(out.str()==expected,"problem input/output adapter");
	};
	run("5\n1 2 1 3 2\n3\n2 5\n1 3\n3 3\n","3\n2\n1\n",solve_p1972);
	run("4 3\n0 0\n2 3\n2 3\n5 1\n0 0 2 3\n2 3 2 3\n3 0 6 2\n",
		"3\n2\n1\n",solve_p2163);
	run("5 6 2 100\n1 2 3 4 5\n1 2\n1 3\n2 4\n2 5\n"
		"2 4 3\n1 4 3 10\n4 2\n3 1 5\n2 5 3\n4 1\n",
		"10\n55\n51\n34\n",solve_hld);
}
void check_monotonic_heaps(){
	auto check=[&](const vector<long long>& a){
		int n=(int)a.size()-1;MonoStack st;
		for(bool small:{true,false}){
			st.init(a,small);
			for(int i=1;i<=n;i++){
				int L=0,R=n+1;
				for(int j=i-1;j>=1;j--) if(small?a[j]<a[i]:a[j]>a[i]){L=j;break;}
				for(int j=i+1;j<=n;j++) if(small?a[j]<a[i]:a[j]>a[i]){R=j;break;}
				require(st.L[i]==L&&st.R[i]==R,"nearest strict neighbors/repeated values");
			}
		}
		st.init(vector<long long>{0,7});
		require(st.L==vector<int>({0,0})&&st.R==vector<int>({2,2}),"monotonic stack reset");
		for(int k=1;k<=n;k++){
			auto [minn,maxn]=window_minmax(a,k);
			require((int)minn.size()==n-k+2&&maxn.size()==minn.size(),"window output indices");
			for(int l=1;l+k-1<=n;l++){
				auto [lo,hi]=minmax_element(a.begin()+l,a.begin()+l+k);
				require(minn[l]==*lo&&maxn[l]==*hi,"window min/max, all lengths");
			}
		}
		MedianHeap heap;heap.init();vector<long long> sorted;
		require(heap.empty()&&heap.size()==0,"empty median heaps");
		for(int i=1;i<=n;i++){
			heap.add(a[i]);sorted.push_back(a[i]);sort(sorted.begin(),sorted.end());
			long long lo=sorted[(i-1)/2],hi=sorted[i/2];
			require(heap.query()==lo&&heap.middle()==make_pair(lo,hi),"prefix lower/upper median");
			require(heap.size()==i&&!heap.empty(),"median element count");
			require(heap.q1.size()==heap.q2.size()||heap.q1.size()==heap.q2.size()+1,
				"median heap size invariant");
			if(!heap.q2.empty()) require(heap.q1.top()<=heap.q2.top(),"median partition order");
		}
		heap.init();heap.add(9);heap.add(-4);
		require(heap.size()==2&&heap.query()==-4,"median reset/even convention");
	};
	// Enumerate short ternary arrays to cover equality, monotonic and alternating cases.
	for(int n=0;n<=5;n++){
		int total=1;for(int i=0;i<n;i++) total*=3;
		for(int mask=0;mask<total;mask++){
			vector<long long> a(n+1);int cur=mask;
			for(int i=1;i<=n;i++){a[i]=cur%3-1;cur/=3;}
			check(a);
		}
	}
	vector<long long> inc(21),dec(21),equal(21,7),zig(21);
	for(int i=1;i<=20;i++){inc[i]=i;dec[i]=-i;zig[i]=i%2?i:-i;}
	check(inc);check(dec);check(equal);check(zig);
	check(vector<long long>{0,LLONG_MIN,LLONG_MAX,0,LLONG_MIN,LLONG_MAX,-1,1});
}
signed main(){
	for(int n:{(int)1,(int)2,(int)5,(int)9,(int)17}) check_ranges(n);
	check_affine();check_versions();check_li_chao();check_dsu();check_mo();check_hld();
	check_dsu_relations();check_odt();check_bit_applications();
	check_monotonic_heaps();
	cout<<"PASS: data structures, "<<checks<<" fixed model/boundary checks"<<endl;
}
