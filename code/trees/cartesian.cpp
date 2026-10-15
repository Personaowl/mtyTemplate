struct CartesianTree{
	int n,root;
	vector<int> l,r,fa;
	void build(const vector<long long> &a,bool small=true){
		n=(int)a.size()-1;root=0;
		l.assign(n+1,0);r.assign(n+1,0);fa.assign(n+1,0);
		vector<int> st;
		for(int i=1;i<=n;i++){
			int last=0;
			while(!st.empty()&&(small?a[st.back()]>a[i]:a[st.back()]<a[i])){
				last=st.back();st.pop_back();
			}
			if(!st.empty()){r[st.back()]=i;fa[i]=st.back();}
			if(last){l[i]=last;fa[last]=i;}
			st.push_back(i);
		}
		if(!st.empty()) root=st[0];
	}
};
