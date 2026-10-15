// Dependency: point.cpp (ld, GEO_EPS, GEO_ANG).
struct Point3{
	ld x=0,y=0,z=0;
	// 三维向量相加／平移点。
	Point3 operator+(const Point3& p)const{return {x+p.x,y+p.y,z+p.z};}
	// 三维两点相减得到位移向量。
	Point3 operator-(const Point3& p)const{return {x-p.x,y-p.y,z-p.z};}
	// 三维向量乘实数。
	Point3 operator*(ld k)const{return {x*k,y*k,z*k};}
	// 三维向量除实数，要求 k 非零。
	Point3 operator/(ld k)const{return {x/k,y/k,z/k};}
};
// 三维向量点积。
ld dot(Point3 a,Point3 b){return a.x*b.x+a.y*b.y+a.z*b.z;}
// 三维叉积向量，方向遵循右手法则。
Point3 cross(Point3 a,Point3 b){return {a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};}
// 三维向量长度。
ld length(Point3 p){return hypotl(hypotl(p.x,p.y),p.z);}
// 混合积 a·(b×c)，即有向平行六面体体积。
ld triple(Point3 a,Point3 b,Point3 c){return dot(a,cross(b,c));}
