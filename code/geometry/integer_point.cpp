// Exact predicates. Input coordinates: |x|,|y|<=1e12; differences also fit long long.
struct IPoint{
	long long x=0,y=0;
	// 向量相加，也可用于平移点；坐标加法须在 long long 内。
	IPoint operator+(const IPoint& p)const{return {x+p.x,y+p.y};}
	// 两点相减得到位移向量。
	IPoint operator-(const IPoint& p)const{return {x-p.x,y-p.y};}
	// 按 (x,y) 精确字典序排序。
	bool operator<(const IPoint& p)const{return tie(x,y)<tie(p.x,p.y);}
	// 精确比较两点坐标是否相同。
	bool operator==(const IPoint& p)const{return x==p.x&&y==p.y;}
};
// 向量点积，乘法前提升到 128 位。
__int128 dot(IPoint a,IPoint b){return (__int128)a.x*b.x+(__int128)a.y*b.y;}
// 二维向量叉积，返回有符号平行四边形面积。
__int128 cross(IPoint a,IPoint b){return (__int128)a.x*b.y-(__int128)a.y*b.x;}
// 判断 a→b 到 c 的转向：正为左侧，负为右侧，0 为共线。
__int128 orient(IPoint a,IPoint b,IPoint c){return cross(b-a,c-a);}
// 两点距离的平方，不开根号。
__int128 dist2(IPoint a,IPoint b){return dot(a-b,a-b);}
// 128 位数的符号：-1、0、1。
int sgn128(__int128 x){return (x>0)-(x<0);}
// 极角分区：零向量 -1，上半平面（含正 x 轴）0，其余 1。
int polar_half(IPoint p){
	if(p.x==0&&p.y==0) return -1;
	return p.y>0||(p.y==0&&p.x>0)?0:1;
}
// 绕原点逆时针极角排序；零向量在前，同射线按长度递增。
bool polar_less(IPoint a,IPoint b){
	int x=polar_half(a),y=polar_half(b);
	if(x!=y) return x<y;
	__int128 v=cross(a,b);
	if(v!=0) return v>0;
	return dot(a,a)<dot(b,b);
}
