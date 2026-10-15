// Dependencies: integer_point.cpp, segment.cpp. Strict CCW hull from convex_hull.
// 严格逆时针凸包内二分：-1 外部、0 边界、1 内部；允许退化凸包。
int point_in_convex(const vector<IPoint>& p,IPoint q){
	int n=p.size();
	if(n==0) return -1;
	if(n==1) return q==p[0]?0:-1;
	if(n==2) return on_segment(q,p[0],p[1])?0:-1;
	__int128 a=orient(p[0],p[1],q),b=orient(p[0],p[n-1],q);
	if(a<0||b>0) return -1;
	if(a==0) return on_segment(q,p[0],p[1])?0:-1;
	if(b==0) return on_segment(q,p[0],p[n-1])?0:-1;
	int l=1,r=n-1;
	while(r-l>1){
		int mid=(l+r)>>1;
		if(orient(p[0],p[mid],q)>=0) l=mid;
		else r=mid;
	}
	return sgn128(orient(p[l],p[r],q));
}
