// Dependency: point.cpp. Translate to p[0] to reduce cancellation at large offsets.
// 有符号两倍面积：平移到首点，配合补偿求和减小消减误差。
ld polygon_area2(const vector<Point>& p){
	ld s=0,err=0;int n=p.size();
	for(int i=1;i+1<n;i++){
		ld v=cross(p[i]-p[0],p[i+1]-p[0])-err,t=s+v;
		err=(t-s)-v;s=t;
	}
	return s;
}
// 均匀多边形薄片的重心；少于三点或计算面积为 0 时返回空。
optional<Point> polygon_centroid(const vector<Point>& p){
	if(p.size()<3) return nullopt;
	ld s=polygon_area2(p);
	if(s==0) return nullopt;
	Point ans{},err{};
	for(int i=1;i+1<(int)p.size();i++){
		Point a=p[i]-p[0],b=p[i+1]-p[0];
		Point val=(a+b)*cross(a,b)-err,t=ans+val;
		err=(t-ans)-val;ans=t;
	}
	return p[0]+ans/(3*s);
}
