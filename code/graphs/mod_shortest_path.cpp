// Reference model: https://oi-wiki.org/graph/mod-shortest-path/
struct ModShortestPath{
	static constexpr long long INF=4e18;
	struct Node{
		long long d;
		int u;
		bool operator<(const Node &a)const{return d>a.d;}
	};
	int p;
	vector<long long> dis;
	void build(const vector<long long> &a){
		p=*min_element(a.begin(),a.end());
		dis.assign(p,INF);
		priority_queue<Node> q;
		dis[0]=0;q.push({0,0});
		while(!q.empty()){
			auto [d,u]=q.top();q.pop();
			if(d!=dis[u]) continue;
			for(auto w:a){
				if(w%p==0) continue;
				int v=(u+w)%p;
				__int128 nd=(__int128)d+w;
				if(nd<dis[v]){
					dis[v]=nd;
					q.push({dis[v],v});
				}
			}
		}
	}
	bool can(long long x)const{
		return x>=0&&dis[x%p]<=x;
	}
	long long count(long long x)const{
		if(x<0) return 0;
		long long ans=0;
		for(auto d:dis) if(d<=x) ans+=(x-d)/p+1;
		return ans;
	}
	long long count(long long l,long long r)const{
		if(l>r||r<0) return 0;
		return count(r)-(l<=0?0:count(l-1));
	}
};
