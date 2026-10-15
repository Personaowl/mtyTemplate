// Requires scc.cpp; component IDs are assigned in reverse topological order.
struct TwoSAT{
	int n;
	SCC scc;
	vector<int> ans;
	void init(int n){this->n=n;scc.init(2*n);ans.clear();}
	int id(int x,bool a) const{return 2*x-1+a;}
	void either(int x,bool a,int y,bool b){
		scc.add(id(x,!a),id(y,b));
		scc.add(id(y,!b),id(x,a));
	}
	void imply(int x,bool a,int y,bool b){either(x,!a,y,b);}
	void force(int x,bool a){either(x,a,x,a);}
	bool solve(){
		scc.build();ans.assign(n+1,0);
		for(int i=1;i<=n;i++){
			if(scc.bel[id(i,false)]==scc.bel[id(i,true)]){
				ans.clear();return false;
			}
			ans[i]=scc.bel[id(i,true)]<scc.bel[id(i,false)];
		}
		return true;
	}
};
