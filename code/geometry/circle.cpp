// Dependencies: point.cpp, line.cpp. Radius >= 0; r<0 is reserved for an invalid circle.
struct Circle{Point o;ld r=-1;};
struct CircleInter{int cnt;vector<Point> p;};
// 圆与无限直线的交点，返回 0／1／2 个点。
vector<Point> circle_line(const Circle& c,const Line& l){
	Point p=projection(c.o,l);ld d=distance(p,c.o);
	if(d>c.r+GEO_EPS) return {};
	if(fabsl(d-c.r)<=GEO_EPS) return {p};
	ld h=sqrtl(max(0.0L,(c.r-d)*(c.r+d)));
	return {p+l.v*h,p-l.v*h};
}
// 两圆交点；cnt=-1 为正半径圆重合，否则 cnt 等于交点数。
CircleInter circle_intersection(const Circle& a,const Circle& b){
	ld d=distance(a.o,b.o);
	if(a.r==0) return fabsl(d-b.r)<=GEO_EPS?CircleInter{1,{a.o}}:CircleInter{0,{}};
	if(b.r==0) return fabsl(d-a.r)<=GEO_EPS?CircleInter{1,{b.o}}:CircleInter{0,{}};
	if(d<=GEO_EPS) return {fabsl(a.r-b.r)<=GEO_EPS?-1:0,{}};
	if(d>a.r+b.r+GEO_EPS||d<fabsl(a.r-b.r)-GEO_EPS) return {0,{}};
	Point v=(b.o-a.o)/d;
	if(fabsl(d-a.r-b.r)<=GEO_EPS) return {1,{a.o+v*a.r}};
	if(fabsl(d-fabsl(a.r-b.r))<=GEO_EPS){
		return {1,{a.o+v*(a.r>=b.r?a.r:-a.r)}};
	}
	ld x=(d+(a.r-b.r)*(a.r+b.r)/d)/2;
	Point p=a.o+v*x;
	ld h=sqrtl(max(0.0L,(a.r-x)*(a.r+x)));
	return {2,{p+perp(v)*h,p-perp(v)*h}};
}
// 两个闭圆盘的公共面积，包含相离、相切和包含情况。
ld circle_overlap(const Circle& a,const Circle& b){
	ld d=distance(a.o,b.o),r=min(a.r,b.r);
	if(d>=a.r+b.r) return 0;
	if(d<=fabsl(a.r-b.r)) return GEO_PI*r*r;
	ld x=(d+(a.r-b.r)*(a.r+b.r)/d)/2;
	ld h=sqrtl(max(0.0L,(a.r-x)*(a.r+x)));
	ld area=a.r*a.r*atan2l(h,x)+b.r*b.r*atan2l(h,d-x)-d*h;
	return max(0.0L,min(GEO_PI*r*r,area));
}
