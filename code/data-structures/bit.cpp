// Following daily-problems/1-16/qoj2693L.cpp; init also clears reused objects.
struct BIT{
	int n;
	vector<long long> tree;
	void init(int n){this->n=n;tree.assign(n+1,0);}
	void add(int x,long long d){
		for(int i=x;i<=n;i+=i&-i) tree[i]+=d;
	}
	long long query(int x)const{
		long long ans=0;
		for(int i=x;i>=1;i-=i&-i) ans+=tree[i];
		return ans;
	}
	long long sum(int l,int r)const{return query(r)-query(l-1);}
	int kth(long long k)const{
		int x=0,step=1;
		while(step<=n/2) step<<=1;
		for(;step;step>>=1){
			if(x+step<=n&&tree[x+step]<k){
				k-=tree[x+step];x+=step;
			}
		}
		return x+1;
	}
};
