// Ordered disjoint intervals; split the right boundary before the left boundary.
struct ODT{
	struct Node{
		int l,r;
		mutable long long v;
		bool operator<(const Node& other)const{return l<other.l;}
	};
	int n;
	long long ans;
	set<Node> tr;
	void init(int n,long long val=0){
		this->n=n;tr.clear();tr.insert({1,n,val});
		ans=(__int128)n*val;
	}
	void init(const vector<long long>& a){
		n=(int)a.size()-1;tr.clear();__int128 sum=0;
		for(int l=1,r;l<=n;l=r+1){
			r=l;while(r<n&&a[r+1]==a[l]) r++;
			tr.insert({l,r,a[l]});sum+=(__int128)(r-l+1)*a[l];
		}
		ans=sum;
	}
	set<Node>::iterator split(int pos){
		if(pos==n+1) return tr.end();
		auto it=tr.lower_bound({pos,0,0});
		if(it!=tr.end()&&it->l==pos) return it;
		--it;auto [l,r,v]=*it;tr.erase(it);
		tr.insert({l,pos-1,v});return tr.insert({pos,r,v}).first;
	}
	void assign(int l,int r,long long val){
		auto itr=split(r+1),itl=split(l);__int128 res=ans;
		for(auto it=itl;it!=itr;it++) res-=(__int128)(it->r-it->l+1)*it->v;
		tr.erase(itl,itr);tr.insert({l,r,val});
		ans=res+(__int128)(r-l+1)*val;
	}
	void add(int l,int r,long long val){
		auto itr=split(r+1),itl=split(l);
		for(auto it=itl;it!=itr;it++) it->v+=val;
		ans=(__int128)ans+(__int128)(r-l+1)*val;
	}
	long long query(int l,int r){
		auto itr=split(r+1),itl=split(l);__int128 res=0;
		for(auto it=itl;it!=itr;it++) res+=(__int128)(it->r-it->l+1)*it->v;
		return res;
	}
};
