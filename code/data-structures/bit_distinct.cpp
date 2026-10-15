// P1972. Dependency: bit.cpp. a is 1-based; answers follow the 0-based query order.
vector<int> count_distinct(const vector<int>& a,const vector<pair<int,int>>& ask){
	struct Query{int l,r,id;};
	int n=(int)a.size()-1,m=ask.size();
	vector<Query> q;
	for(int i=0;i<m;i++) q.push_back({ask[i].first,ask[i].second,i});
	sort(q.begin(),q.end(),[](const Query& x,const Query& y){return x.r<y.r;});
	int maxn=*max_element(a.begin()+1,a.end());
	vector<int> last(maxn+1,0),ans(m);
	BIT bit;bit.init(n);int r=0;
	for(auto [L,R,id]:q){
		while(r<R){
			r++;int x=a[r];
			if(last[x]) bit.add(last[x],-1);
			bit.add(r,1);last[x]=r;
		}
		ans[id]=bit.sum(L,R);
	}
	return ans;
}
void solve_p1972(){
	int n,m;cin>>n;vector<int> a(n+1);
	for(int i=1;i<=n;i++) cin>>a[i];
	cin>>m;vector<pair<int,int>> ask(m);
	for(auto& [l,r]:ask) cin>>l>>r;
	for(auto x:count_distinct(a,ask)) cout<<x<<'\n';
}
