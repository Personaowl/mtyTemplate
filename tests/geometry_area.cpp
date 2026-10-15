#include<bits/stdc++.h>
using namespace std;
#ifdef MTY_INT64
#define int long long
#endif
#include "code/geometry/integer_point.cpp"
#include "code/geometry/segment.cpp"
#include "code/geometry/polygon.cpp"

long long checks=0;
void require(bool ok){
	checks++;
	if(!ok){cerr<<"geometry area check failed: "<<checks<<'\n';exit(1);}
}
// 独立按首点扇形三角剖分；展开三角形行列式，避免复用面积循环。
__int128 fan_area2(const vector<IPoint>& p){
	__int128 ans=0;
	for(int i=2;i<(int)p.size();i++){
		__int128 ax=p[i-1].x-p[0].x,ay=p[i-1].y-p[0].y;
		__int128 bx=p[i].x-p[0].x,by=p[i].y-p[0].y;
		ans+=ax*by-ay*bx;
	}
	return ans;
}
void verify(vector<IPoint> p,__int128 expected){
	require(polygon_area2(p)==expected);
	require(polygon_area2(p)==fan_area2(p));
	reverse(p.begin(),p.end());
	require(polygon_area2(p)==-expected);
	for(auto& v:p){v.x+=700000000000LL;v.y-=600000000000LL;}
	require(polygon_area2(p)==-expected);
}
signed main(){
	verify({},0);verify({{3,4}},0);verify({{0,0},{3,4}},0);
	verify({{0,0},{1,0},{0,1}},1);
	verify({{0,0},{4,0},{4,3},{0,3}},24);
	// 5×4 矩形去掉右上方 3×3 小矩形，得到凹多边形。
	verify({{0,0},{5,0},{5,1},{2,1},{2,4},{0,4}},22);
	verify({{0,0},{1,0},{2,0},{3,0}},0);
	mt19937_64 rng(20261015);
	for(int t=0;t<2000;t++){
		int n=rng()%40;vector<IPoint> p(n);
		for(auto& v:p){v.x=(long long)(rng()%2000001)-1000000;v.y=(long long)(rng()%2000001)-1000000;}
		// 无序闭边界也应保持代数面积恒等式，不把它当作区域并面积。
		verify(p,fan_area2(p));
	}
	const long long C=1000000000000LL,step=40000000;
	vector<IPoint> p;
	for(long long x=-C;x<C;x+=step) p.push_back({x,-C});
	for(long long y=-C;y<C;y+=step) p.push_back({C,y});
	for(long long x=C;x>-C;x-=step) p.push_back({x,C});
	for(long long y=C;y>-C;y-=step) p.push_back({-C,y});
	require(p.size()==200000);
	__int128 expected=(__int128)8*C*C;
	require(polygon_area2(p)==expected);
	require(fan_area2(p)==expected);
	reverse(p.begin(),p.end());
	require(polygon_area2(p)==-expected);
	cout<<"geometry area: "<<checks<<" checks passed\n";
}
