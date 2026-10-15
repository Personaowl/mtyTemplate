#include<bits/stdc++.h>
#define endl '\n'
using namespace std;
struct RandomData{
	mt19937_64 rng;
	RandomData(uint64_t seed):rng(seed){}
	long long rnd(long long l,long long r){
		return uniform_int_distribution<long long>(l,r)(rng);
	}
	vector<pair<int,int>> tree(int n){
		vector<pair<int,int>> e;
		for(int i=2;i<=n;i++) e.push_back({(int)rnd(1,i-1),i});
		return e;
	}
	// 小规模连通简单无向图，n-1<=m<=n(n-1)/2；枚举候选边保证终止。
	vector<pair<int,int>> graph(int n,int m){
		auto e=tree(n);set<pair<int,int>> used(e.begin(),e.end());
		vector<pair<int,int>> rest;
		for(int i=1;i<=n;i++) for(int j=i+1;j<=n;j++){
			if(!used.count({i,j})) rest.push_back({i,j});
		}
		shuffle(rest.begin(),rest.end(),rng);
		for(int i=0;(int)e.size()<m;i++) e.push_back(rest[i]);
		shuffle(e.begin(),e.end(),rng);return e;
	}
};
int main(int argc,char** argv){
	ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);
	uint64_t seed=argc>1?stoull(argv[1]):chrono::steady_clock::now().time_since_epoch().count();
	RandomData gen(seed);
	int n=gen.rnd(1,10);
	cout<<1<<endl; // 多测题的 T；单测题删掉本行
	cout<<n<<endl;
	for(int i=1;i<=n;i++) cout<<gen.rnd(-10,10)<<" \n"[i==n];
	return 0;
}
