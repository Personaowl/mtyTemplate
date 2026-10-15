struct AC{
	struct Node{
		array<int32_t,26> son{};
		int32_t fail=0;
	};
	vector<Node> tr;
	vector<int> pos,ord;
	bool built=false;
	AC(){init();}
	void init(){
		tr.assign(1,{});
		pos.assign(1,0);
		ord.clear();
		built=false;
	}
	int insert(const string &s){
		int u=0;
		for(auto ch:s){
			int c=ch-'a';
			if(!tr[u].son[c]){
				int v=tr.size();
				tr[u].son[c]=v;
				tr.push_back({});
			}
			u=tr[u].son[c];
		}
		pos.push_back(u);
		return (int)pos.size()-1;
	}
	void build(){
		if(built) return;
		built=true;
		queue<int> q;
		for(int c=0;c<26;c++){
			if(tr[0].son[c]) q.push(tr[0].son[c]);
		}
		while(!q.empty()){
			int u=q.front();q.pop();
			ord.push_back(u);
			for(int c=0;c<26;c++){
				int v=tr[u].son[c];
				if(v){
					tr[v].fail=tr[tr[u].fail].son[c];
					q.push(v);
				}
				else tr[u].son[c]=tr[tr[u].fail].son[c];
			}
		}
	}
	vector<long long> query(const string &s){
		vector<long long> cnt(tr.size(),0),ans(pos.size(),0);
		int u=0;
		for(auto ch:s){
			int c=ch-'a';
			u=tr[u].son[c];
			cnt[u]++;
		}
		for(int i=(int)ord.size()-1;i>=0;i--){
			int v=ord[i];
			cnt[tr[v].fail]+=cnt[v];
		}
		cnt[0]=(long long)s.size()+1;
		for(int i=1;i<(int)pos.size();i++) ans[i]=cnt[pos[i]];
		return ans;
	}
};
