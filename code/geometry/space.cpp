// Dependency: point3.cpp. Plane normal is normalized; normal must be nonzero.
struct Plane{
	Point3 p,n;
	// 由平面上一点和非零法向量构造平面，并单位化法向量。
	Plane(Point3 p,Point3 normal):p(p),n(normal/length(normal)){}
	// 点到平面的有符号距离，法向量所指一侧为正。
	ld side(Point3 q)const{return dot(n,q-p);}
};
// 三维点到平面的正交投影。
Point3 projection(Point3 p,const Plane& f){return p-f.n*f.side(p);}
struct PlaneInter{int kind;Point3 p;};
// 无限直线与平面求交：0 平行无交，1 唯一交点，2 直线在面内。
PlaneInter line_plane(Point3 a,Point3 b,const Plane& f){
	Point3 v=(b-a)/length(b-a);ld d=dot(v,f.n);
	if(fabsl(d)<=GEO_ANG) return {fabsl(f.side(a))<=GEO_EPS?2:0,{}};
	return {1,a-v*(f.side(a)/d)};
}
// 两条空间无限直线的最短距离；某一对端点重合时按点处理。
ld space_line_distance(Point3 a,Point3 b,Point3 c,Point3 d){
	Point3 u=b-a,v=d-c;ld x=length(u),y=length(v);
	if(x==0&&y==0) return length(a-c);
	if(x==0) return length(cross(a-c,v))/y;
	if(y==0) return length(cross(c-a,u))/x;
	u=u/x;v=v/y;Point3 n=cross(u,v);ld z=length(n);
	if(z<=GEO_ANG) return length(cross(c-a,u));
	return fabsl(dot(c-a,n))/z;
}
