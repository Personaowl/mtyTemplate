// Dependency: integer_point.cpp. Closed segments, including zero-length ones.
// 判断点 p 是否在闭线段 ab 上，允许 a=b。
bool on_segment(IPoint p,IPoint a,IPoint b){
	return orient(a,b,p)==0&&dot(p-a,p-b)<=0;
}
// 闭线段相交分类：0 无交，1 单点相交，2 正长度重合。
int segment_relation(IPoint a,IPoint b,IPoint c,IPoint d){
	if(max(a.x,b.x)<min(c.x,d.x)||max(c.x,d.x)<min(a.x,b.x)
		||max(a.y,b.y)<min(c.y,d.y)||max(c.y,d.y)<min(a.y,b.y)) return 0;
	int x=sgn128(orient(a,b,c)),y=sgn128(orient(a,b,d));
	int z=sgn128(orient(c,d,a)),w=sgn128(orient(c,d,b));
	if(x==0&&y==0&&z==0&&w==0){
		if(b<a) swap(a,b);
		if(d<c) swap(c,d);
		IPoint l=max(a,c),r=min(b,d);
		if(r<l) return 0;
		return l==r?1:2;
	}
	return x*y<=0&&z*w<=0;
}
