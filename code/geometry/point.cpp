using ld=long double;
constexpr ld GEO_EPS=1e-10L;
constexpr ld GEO_ANG=1e-18L;
const ld GEO_PI=acosl(-1.0L);
struct Point{
	ld x=0,y=0;
	// 向量相加／平移点。
	Point operator+(const Point& p)const{return {x+p.x,y+p.y};}
	// 两点相减得到位移向量。
	Point operator-(const Point& p)const{return {x-p.x,y-p.y};}
	// 向量乘实数；缩放相对于原点。
	Point operator*(ld k)const{return {x*k,y*k};}
	// 向量除实数，要求 k 非零。
	Point operator/(ld k)const{return {x/k,y/k};}
	// 精确字典序，仅用于排序，不加入 EPS。
	bool operator<(const Point& p)const{return tie(x,y)<tie(p.x,p.y);}
};
// 二维向量点积。
ld dot(Point a,Point b){return a.x*b.x+a.y*b.y;}
// 二维向量叉积，正值表示逆时针转向。
ld cross(Point a,Point b){return a.x*b.y-a.y*b.x;}
// 向量长度平方。
ld norm2(Point p){return dot(p,p);}
// 向量长度，使用 hypotl。
ld length(Point p){return hypotl(p.x,p.y);}
// 两点间的欧氏距离。
ld distance(Point a,Point b){return length(a-b);}
// 向量逆时针旋转 90 度。
Point perp(Point p){return {-p.y,p.x};}
// 绕原点旋转 rad 弧度，正值为逆时针。
Point rotate_point(Point p,ld rad){
	ld c=cosl(rad),s=sinl(rad);return {p.x*c-p.y*s,p.x*s+p.y*c};
}
// 按长度绝对容差判断两点是否接近。
bool same_point(Point a,Point b){return distance(a,b)<=GEO_EPS;}
// 按指定绝对容差返回 -1、0、1；eps 单位须与 x 相同。
int geo_sgn(ld x,ld eps=GEO_EPS){return (x>eps)-(x<-eps);}
// 将数值截到 [-1,1]，供合法反三角公式消除舍入越界。
ld clamp_unit(ld x){return max(-1.0L,min(1.0L,x));}
// 非零向量 a 到 b 的有向夹角，范围 [-pi,pi]。
ld angle(Point a,Point b){return atan2l(cross(a,b),dot(a,b));}
