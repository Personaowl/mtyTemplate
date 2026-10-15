// Dependency: point3.cpp. Latitude in [-pi/2,pi/2], longitude in radians, R>=0.
// 经纬度弧度输入，返回半径 R 球面上的最短大圆弧长。
ld great_circle_distance(ld lat1,ld lon1,ld lat2,ld lon2,ld R){
	// 将经纬度转换成单位球面上的三维向量。
	auto unit=[](ld lat,ld lon)->Point3{
		return {cosl(lat)*cosl(lon),cosl(lat)*sinl(lon),sinl(lat)};
	};
	Point3 a=unit(lat1,lon1),b=unit(lat2,lon2);
	return R*atan2l(length(cross(a,b)),dot(a,b));
}
