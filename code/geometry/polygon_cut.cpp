// Dependencies: point.cpp, line.cpp, polygon_real.cpp. Keep the closed left half-plane.
// 仅删除相邻近重点和末尾重复首点，不排序、不求凸包。
vector<Point> clean_polygon(const vector<Point>& p){
	vector<Point> q;
	for(auto v:p) if(q.empty()||!same_point(q.back(),v)) q.push_back(v);
	if(q.size()>1&&same_point(q.front(),q.back())) q.pop_back();
	return q;
}
// 用有向直线的闭左半平面裁剪凸多边形，保留点／线段退化结果。
vector<Point> cut_polygon(const vector<Point>& p,const Line& l){
	vector<Point> q;int n=p.size();
	for(int i=0;i<n;i++){
		Point a=p[i],b=p[(i+1)%n];ld x=l.side(a),y=l.side(b);
		if(fabsl(x)<=GEO_EPS) x=0;
		if(fabsl(y)<=GEO_EPS) y=0;
		if(x>=0) q.push_back(a);
		if((x>=0)!=(y>=0)) q.push_back(a+(b-a)*(x/(x-y)));
	}
	return clean_polygon(q);
}
// 两个凸多边形求交；裁剪边界 q 须非退化，允许顺／逆时针。
vector<Point> convex_intersection(vector<Point> p,vector<Point> q){
	if(q.size()<3) return {};
	if(polygon_area2(q)<0) reverse(q.begin(),q.end());
	for(int i=0;i<(int)q.size()&&!p.empty();i++){
		Point a=q[i],b=q[(i+1)%q.size()];
		if(!same_point(a,b)) p=cut_polygon(p,Line(a,b));
	}
	return p;
}
