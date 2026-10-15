// Dependencies: integer_point.cpp, segment.cpp. Cyclic vertices, no repeated last vertex.
// 鞋带公式：按循环边累加叉积，返回有符号两倍面积。
__int128 polygon_area2(const vector<IPoint>& p){
	__int128 s=0;int n=p.size();
	for(int i=0;i<n;i++) s+=cross(p[i],p[(i+1)%n]);
	return s;
}
// 简单多边形点包含：-1 外部、0 边界、1 内部；顺逆时针均可。
int point_in_polygon(const vector<IPoint>& p,IPoint q){
	int n=p.size(),wn=0;
	for(int i=0;i<n;i++){
		IPoint a=p[i],b=p[(i+1)%n];
		if(on_segment(q,a,b)) return 0;
		__int128 v=orient(a,b,q);
		if(a.y<=q.y&&b.y>q.y&&v>0) wn++;
		if(a.y>q.y&&b.y<=q.y&&v<0) wn--;
	}
	return wn?1:-1;
}
// Pick 定理返回 {内部格点数,边界格点数}；要求非退化、无孔整点多边形。
pair<__int128,__int128> lattice_points(const vector<IPoint>& p){
	__int128 a=polygon_area2(p),b=0;
	if(a<0) a=-a;
	int n=p.size();
	for(int i=0;i<n;i++){
		IPoint d=p[(i+1)%n]-p[i];b+=gcd(llabs(d.x),llabs(d.y));
	}
	return {(a-b+2)/2,b};
}
