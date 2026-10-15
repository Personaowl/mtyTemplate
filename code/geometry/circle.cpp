// Dependencies: point.cpp, line.cpp. Radius >= 0; r<0 is reserved for an invalid circle.
struct Circle{Point o;ld r=-1;};
struct CircleInter{int cnt;vector<Point> p;};
// 两点相交时求两圆心到公共弦的有向距离 x/y、半弦长 h；不用于分类。
array<ld,3> circle_chord(ld r,ld s,ld d){
	array<ld,3> len={r,s,d};sort(len.begin(),len.end());
	ld a=len[2],b=len[1],c=len[0];
	// Kahan 重排海伦公式，避免直接计算 r*r-x*x 丢失小圆尺度。
	ld h=sqrtl(max(0.0L,(a+(b+c))*(c-(a-b))*(c+(a-b))*(a+(b-c))))/(2*d);
	// 按尺度选择等价公式，减少公共弦投影距离中的大数相消。
	auto along=[&](ld r,ld s){
		if(d>max(r,s)/2) return ((d-s)*(d+s)+r*r)/(2*d);
		return (d+(r-s)*(r+s)/d)/2;
	};
	return {along(r,s),along(s,r),h};
}
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
	auto [x,y,h]=circle_chord(a.r,b.r,d);
	Point p=a.r<=b.r?a.o+v*x:b.o-v*y;
	return {2,{p+perp(v)*h,p-perp(v)*h}};
}
// 两个闭圆盘的公共面积，包含相离、相切和包含情况。
ld circle_overlap(const Circle& a,const Circle& b){
	ld d=distance(a.o,b.o),r=min(a.r,b.r);
	if(d>=a.r+b.r) return 0;
	if(d<=fabsl(a.r-b.r)) return GEO_PI*r*r;
	auto [x,y,h]=circle_chord(a.r,b.r,d);
	// 单个圆弓形面积；小角用展开式，避免扇形与三角形几乎相消。
	auto segment=[](ld r,ld x,ld h){
		ld t=atan2l(h,x);
		if(t<1e-3L){
			ld z=t*t;
			return r*r*t*z*(2.0L/3-z*(2.0L/15-z*(4.0L/315-z*2.0L/2835)));
		}
		return r*r*t-x*h;
	};
	ld area=segment(a.r,x,h)+segment(b.r,y,h);
	return max(0.0L,min(GEO_PI*r*r,area));
}
