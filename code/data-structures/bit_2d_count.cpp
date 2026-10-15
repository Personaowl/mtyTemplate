// P2163. Dependency: bit.cpp. Closed rectangles and repeated points are supported.
vector<long long> count_points(vector<pair<int,int>> p,
		const vector<array<int,4>>& ask){
	struct Event{int x,y,id,sign;};
	vector<Event> q;int m=ask.size();q.reserve(m*4);
	for(int i=0;i<m;i++){
		auto [a,b,c,d]=ask[i];
		q.push_back({c,d,i,1});q.push_back({a-1,d,i,-1});
		q.push_back({c,b-1,i,-1});q.push_back({a-1,b-1,i,1});
	}
	vector<int> ys;ys.reserve(p.size());
	for(auto [x,y]:p) ys.push_back(y);
	sort(ys.begin(),ys.end());ys.erase(unique(ys.begin(),ys.end()),ys.end());
	sort(p.begin(),p.end());
	sort(q.begin(),q.end(),[](const Event& x,const Event& y){return x.x<y.x;});
	BIT bit;bit.init(ys.size());vector<long long> ans(m);int j=0;
	for(auto [x,y,id,sign]:q){
		while(j<(int)p.size()&&p[j].first<=x){
			int pos=lower_bound(ys.begin(),ys.end(),p[j].second)-ys.begin()+1;
			bit.add(pos,1);j++;
		}
		int pos=upper_bound(ys.begin(),ys.end(),y)-ys.begin();
		ans[id]+=sign*bit.query(pos);
	}
	return ans;
}
void solve_p2163(){
	int n,m;cin>>n>>m;
	vector<pair<int,int>> p(n);vector<array<int,4>> ask(m);
	for(auto& [x,y]:p) cin>>x>>y;
	for(auto& [a,b,c,d]:ask) cin>>a>>b>>c>>d;
	for(auto x:count_points(p,ask)) cout<<x<<'\n';
}
