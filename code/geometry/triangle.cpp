// Dependencies: point.cpp, circle.cpp. Near-collinear triangles return nullopt.
// 三角形外接圆；重复点、共线或近共线时返回空。
optional<Circle> circumcircle(Point a,Point b,Point c){
	Point u=b-a,v=c-a;ld d=cross(u,v);
	if(fabsl(d)<=GEO_ANG*length(u)*length(v)) return nullopt;
	Point o=a+(perp(v)*(-norm2(u))+perp(u)*norm2(v))/(2*d);
	return Circle{o,distance(o,a)};
}
// 三角形内切圆；重复点、共线或近共线时返回空。
optional<Circle> incircle(Point a,Point b,Point c){
	ld x=distance(b,c),y=distance(a,c),z=distance(a,b),s=x+y+z;
	ld d=fabsl(cross(b-a,c-a));
	if(d<=GEO_ANG*y*z) return nullopt;
	return Circle{(a*x+b*y+c*z)/s,d/s};
}
