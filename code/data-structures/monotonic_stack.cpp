// L/R: nearest strictly smaller (small=true) or strictly greater positions.
struct MonoStack{
	int n;
	vector<int> L,R;
	void init(const vector<long long>& a,bool small=true){
		n=(int)a.size()-1;L.assign(n+1,0);R.assign(n+1,n+1);
		vector<int> st;
		auto bad=[&](int x,int y){return small?a[x]>=a[y]:a[x]<=a[y];};
		for(int i=1;i<=n;i++){
			while(!st.empty()&&bad(st.back(),i)) st.pop_back();
			if(!st.empty()) L[i]=st.back();
			st.push_back(i);
		}
		st.clear();
		for(int i=n;i>=1;i--){
			while(!st.empty()&&bad(st.back(),i)) st.pop_back();
			if(!st.empty()) R[i]=st.back();
			st.push_back(i);
		}
	}
};
