// Dependency: integer_point.cpp. Andrew hull; exact turns, duplicate input allowed.
// Andrew 严格凸包：去重、去边上中间共线点，逆时针返回；不修改原点集。
vector<IPoint> convex_hull(vector<IPoint> a){
	sort(a.begin(),a.end());a.erase(unique(a.begin(),a.end()),a.end());
	int n=a.size();if(n<=1) return a;
	vector<IPoint> lo,hi;
	for(auto p:a){
		while(lo.size()>=2&&orient(lo[lo.size()-2],lo.back(),p)<=0) lo.pop_back();
		lo.push_back(p);
	}
	for(int i=n-1;i>=0;i--){
		auto p=a[i];
		while(hi.size()>=2&&orient(hi[hi.size()-2],hi.back(),p)<=0) hi.pop_back();
		hi.push_back(p);
	}
	lo.pop_back();hi.pop_back();lo.insert(lo.end(),hi.begin(),hi.end());
	return lo;
}
