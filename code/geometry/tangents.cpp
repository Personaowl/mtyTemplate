// Dependency: circle.cpp. Both circles have strictly positive radius.
struct Tangent{Point a,b,dir;};
struct TangentSet{bool infinite=false;vector<Tangent> lines;};
// 正半径两圆的公切线：inner=false 外公切线，true 内公切线。
TangentSet circle_tangents(const Circle& a,const Circle& b,bool inner=false){
	Point d=b.o-a.o;ld z=norm2(d),len=sqrtl(z),s=inner?-1:1;
	ld dr=a.r-s*b.r;TangentSet ans;
	if(len<=GEO_EPS){ans.infinite=!inner&&fabsl(a.r-b.r)<=GEO_EPS;return ans;}
	if(len<fabsl(dr)-GEO_EPS) return ans;
	ld h=sqrtl(max(0.0L,(len-fabsl(dr))*(len+fabsl(dr))));
	int cnt=fabsl(len-fabsl(dr))<=GEO_EPS?1:2;
	for(int i=0;i<cnt;i++){
		Point n=cnt==1?d*((dr>=0?1.0L:-1.0L)/len):(d*dr+perp(d)*(i?-h:h))/z;
		ans.lines.push_back({a.o+n*a.r,b.o+n*(s*b.r),perp(n)});
	}
	return ans;
}
// 点到正半径圆的切点：圆内 0 个，圆上 1 个，圆外 2 个。
vector<Point> tangent_points(Point p,const Circle& c){
	Point v=p-c.o;ld d=length(v);
	if(d==0) return {};
	if(d<c.r-GEO_EPS) return {};
	if(fabsl(d-c.r)<=GEO_EPS) return {c.o+v*(c.r/d)};
	ld a=acosl(clamp_unit(c.r/d));v=v*(c.r/d);
	return {c.o+rotate_point(v,a),c.o+rotate_point(v,-a)};
}
