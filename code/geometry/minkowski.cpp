// Dependencies: integer_point.cpp, convex_hull.cpp. Input can be arbitrary point sets.
// 先取两组点的凸包，再求两个凸集的 Minkowski 和；不修改输入。
vector<IPoint> minkowski_sum(vector<IPoint> a,vector<IPoint> b){
	a=convex_hull(a);b=convex_hull(b);
	if(a.empty()||b.empty()) return {};
	int n=a.size(),m=b.size();vector<IPoint> c;
	if(n<=2||m<=2){
		for(auto p:a) for(auto q:b) c.push_back(p+q);
		return convex_hull(c);
	}
	// 将凸包循环起点移到最低、再最左的点，便于归并边向量。
	auto start=[](vector<IPoint>& p){
		auto it=min_element(p.begin(),p.end(),[](IPoint a,IPoint b){return tie(a.y,a.x)<tie(b.y,b.x);});
		rotate(p.begin(),it,p.end());
	};
	start(a);start(b);
	int i=0,j=0;
	while(i<n||j<m){
		c.push_back(a[i%n]+b[j%m]);
		if(i==n){j++;continue;}
		if(j==m){i++;continue;}
		IPoint u=a[(i+1)%n]-a[i],v=b[(j+1)%m]-b[j];
		int x=polar_half(u),y=polar_half(v);__int128 z=cross(u,v);
		if(x!=y){if(x<y) i++;else j++;}
		else if(z>0) i++;
		else if(z<0) j++;
		else{i++;j++;}
	}
	return convex_hull(c);
}
