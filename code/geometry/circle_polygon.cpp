// Dependencies: point.cpp, circle.cpp. Simple polygon; either orientation.
// 闭圆盘与简单多边形的公共面积；按边界顺序输入，顺逆时针均可。
ld circle_polygon_area(const Circle& c,const vector<Point>& p){
	if(c.r==0||p.size()<3) return 0;
	ld ans=0,err=0,r2=c.r*c.r;
	for(int i=0;i<(int)p.size();i++){
		Point a=p[i]-c.o,b=p[(i+1)%p.size()]-c.o,d=b-a;
		ld z=norm2(d);if(z==0) continue;
		vector<ld> ts{0,1};
		ld t=-dot(a,d)/z,h2=r2-cross(a,d)*cross(a,d)/z;
		if(h2>0){
			ld h=sqrtl(h2/z);
			if(t-h>0&&t-h<1) ts.push_back(t-h);
			if(t+h>0&&t+h<1) ts.push_back(t+h);
		}
		sort(ts.begin(),ts.end());
		for(int j=1;j<(int)ts.size();j++){
			Point u=a+d*ts[j-1],v=a+d*ts[j],mid=a+d*((ts[j-1]+ts[j])/2);
			ld val=norm2(mid)<=r2?cross(u,v)/2:r2*atan2l(cross(u,v),dot(u,v))/2;
			ld y=val-err,s=ans+y;err=(s-ans)-y;ans=s;
		}
	}
	return fabsl(ans);
}
