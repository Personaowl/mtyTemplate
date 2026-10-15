struct Trie{
	static constexpr int SIG=62;
	vector<array<int32_t,SIG>> tr;
	vector<int> cnt,ed;
	Trie(){init();}
	void init(){
		tr.assign(1,{});
		cnt.assign(1,0);
		ed.assign(1,0);
	}
	int get(char c){
		if(c>='0'&&c<='9') return c-'0';
		if(c>='a'&&c<='z') return c-'a'+10;
		if(c>='A'&&c<='Z') return c-'A'+36;
		return -1;
	}
	void insert(const string &s){
		int u=0;
		cnt[u]++;
		for(auto ch:s){
			int c=get(ch);
			if(!tr[u][c]){
				int v=tr.size();
				tr[u][c]=v;
				tr.push_back({});
				cnt.push_back(0);
				ed.push_back(0);
			}
			u=tr[u][c];
			cnt[u]++;
		}
		ed[u]++;
	}
	int find(const string &s){
		int u=0;
		for(auto ch:s){
			int c=get(ch);
			if(!tr[u][c]) return -1;
			u=tr[u][c];
		}
		return u;
	}
	int query(const string &s){
		int u=find(s);
		return u==-1?0:ed[u];
	}
	int prefix(const string &s){
		int u=find(s);
		return u==-1?0:cnt[u];
	}
	bool erase(const string &s){
		int u=find(s);
		if(u==-1||ed[u]==0) return false;
		ed[u]--;
		u=0;
		cnt[u]--;
		for(auto ch:s){
			u=tr[u][get(ch)];
			cnt[u]--;
		}
		return true;
	}
};
