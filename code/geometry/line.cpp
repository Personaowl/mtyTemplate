// Dependency: point.cpp. a!=b; v is a unit direction, allowed side is the left.
struct Line{
	Point p,v;
	// 由不同两点构造有向直线，自动将方向归一化。
	Line(Point a,Point b):p(a),v((b-a)/length(b-a)){}
	// 点到直线的有符号距离：左正右负。
	ld side(Point q)const{return cross(v,q-p);}
};
struct LineInter{int kind;Point p;};
// 无限直线求交：kind=0 平行异线、1 唯一交点、2 重合。
LineInter line_intersection(const Line& a,const Line& b){
	ld d=cross(a.v,b.v);
	if(fabsl(d)<=GEO_ANG) return {fabsl(a.side(b.p))<=GEO_EPS?2:0,{}};
	return {1,a.p+a.v*(cross(b.p-a.p,b.v)/d)};
}
// 点 p 在无限直线上的垂足。
Point projection(Point p,const Line& l){return l.p+l.v*dot(p-l.p,l.v);}
// 点 p 关于直线的对称点。
Point reflection(Point p,const Line& l){return projection(p,l)*2-p;}
// 点到无限直线的非负距离。
ld point_line_distance(Point p,const Line& l){return fabsl(l.side(p));}
// 点到闭线段的最短距离，允许零长度线段。
ld point_segment_distance(Point p,Point a,Point b){
	Point v=b-a;
	ld z=norm2(v);
	if(z==0) return distance(p,a);
	ld t=max(0.0L,min(1.0L,dot(p-a,v)/z));
	return distance(p,a+v*t);
}
