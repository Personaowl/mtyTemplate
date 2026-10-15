#include<bits/stdc++.h>
#ifdef MTY_INT64
#define int long long
#endif
using namespace std;
using I128=__int128;
using Dec=long double;
#include "code/geometry/integer_point.cpp"
#include "code/geometry/point.cpp"
#include "code/geometry/segment.cpp"
#include "code/geometry/line.cpp"
#include "code/geometry/polygon.cpp"
#include "code/geometry/polygon_real.cpp"
#include "code/geometry/convex_hull.cpp"
#include "code/geometry/convex_query.cpp"
#include "code/geometry/calipers.cpp"
#include "code/geometry/closest_pair.cpp"
#include "code/geometry/minkowski.cpp"
#include "code/geometry/circle.cpp"
#include "code/geometry/tangents.cpp"
#include "code/geometry/triangle.cpp"
#include "code/geometry/enclosing_circle.cpp"
#include "code/geometry/circle_polygon.cpp"
#include "code/geometry/polygon_cut.cpp"
#include "code/geometry/halfplane.cpp"
#include "code/geometry/point3.cpp"
#include "code/geometry/space.cpp"
#include "code/geometry/volume.cpp"
#include "code/geometry/sphere.cpp"

long long checks=0;
mt19937_64 rng(20261015);
void require(bool ok,const string& msg){
	checks++;
	if(!ok){cerr<<"FAIL: "<<msg<<'\n';exit(1);}
}
void near(ld a,ld b,ld tol,const string& msg){
	if(!(isfinite(a)&&isfinite(b)&&fabsl(a-b)<=tol*(1+fabsl(b)))){
		cerr<<setprecision(24)<<a<<" != "<<b<<" tol="<<tol<<'\n';require(false,msg);
	}
	checks++;
}
long long rd(long long l,long long r){return uniform_int_distribution<long long>(l,r)(rng);}
I128 det(IPoint a,IPoint b,IPoint c){
	return (I128(b.x)-a.x)*(I128(c.y)-a.y)-(I128(b.y)-a.y)*(I128(c.x)-a.x);
}
bool model_on(IPoint p,IPoint a,IPoint b){
	return det(a,b,p)==0&&min(a.x,b.x)<=p.x&&p.x<=max(a.x,b.x)
		&&min(a.y,b.y)<=p.y&&p.y<=max(a.y,b.y);
}
int segment_model(IPoint a,IPoint b,IPoint c,IPoint d){
	if(a==b) return model_on(a,c,d);
	if(c==d) return model_on(c,a,b);
	I128 den=(I128(b.x)-a.x)*(I128(d.y)-c.y)-(I128(b.y)-a.y)*(I128(d.x)-c.x);
	if(den==0){
		if(det(a,b,c)!=0) return 0;
		long long l,r;
		if(a.x!=b.x){l=max(min(a.x,b.x),min(c.x,d.x));r=min(max(a.x,b.x),max(c.x,d.x));}
		else{l=max(min(a.y,b.y),min(c.y,d.y));r=min(max(a.y,b.y),max(c.y,d.y));}
		return l>r?0:(l==r?1:2);
	}
	I128 t=(I128(c.x)-a.x)*(I128(d.y)-c.y)-(I128(c.y)-a.y)*(I128(d.x)-c.x);
	I128 u=(I128(c.x)-a.x)*(I128(b.y)-a.y)-(I128(c.y)-a.y)*(I128(b.x)-a.x);
	if(den<0){den=-den;t=-t;u=-u;}
	return t>=0&&t<=den&&u>=0&&u<=den;
}
vector<IPoint> model_hull(vector<IPoint> a){
	sort(a.begin(),a.end());a.erase(unique(a.begin(),a.end()),a.end());
	if(a.size()<=1) return a;
	vector<IPoint> h;IPoint cur=a[0];
	do{
		h.push_back(cur);IPoint next=a[0]==cur?a[1]:a[0];
		for(auto p:a){
			I128 d=det(cur,next,p);
			I128 dx=I128(p.x)-cur.x,dy=I128(p.y)-cur.y;
			I128 nx=I128(next.x)-cur.x,ny=I128(next.y)-cur.y;
			if(d<0||(d==0&&dx*dx+dy*dy>nx*nx+ny*ny)) next=p;
		}
		cur=next;
	}while(!(cur==a[0]));
	return h;
}
int polygon_model(const vector<IPoint>& p,IPoint q){
	bool in=false;int n=p.size();
	for(int i=0;i<n;i++){
		IPoint a=p[i],b=p[(i+1)%n];
		if(model_on(q,a,b)) return 0;
		if((a.y>q.y)!=(b.y>q.y)){
			I128 den=b.y-a.y,num=I128(a.x)*den+I128(q.y-a.y)*(b.x-a.x);
			if(den>0?num>I128(q.x)*den:num<I128(q.x)*den) in=!in;
		}
	}
	return in?1:-1;
}
void integer_checks(){
	vector<IPoint> g;for(int x=-1;x<=1;x++) for(int y=-1;y<=1;y++) g.push_back({x,y});
	for(auto a:g) for(auto b:g) for(auto c:g) for(auto d:g){
		require(segment_relation(a,b,c,d)==segment_model(a,b,c,d),"closed segment rational oracle, incl points/overlap");
	}
	for(int mask=0;mask<(1LL<<g.size());mask++){
		vector<IPoint> a;for(int i=0;i<(int)g.size();i++) if(mask>>i&1) a.push_back(g[i]);
		if(!a.empty()) a.push_back(a[0]);
		auto h=convex_hull(a);require(h==model_hull(a),"Andrew vs gift wrapping");
		__int128 mn=-1,mx=h.empty()?-1:0;
		for(int i=0;i<(int)a.size();i++) for(int j=i+1;j<(int)a.size();j++){
			__int128 z=dist2(a[i],a[j]);mn=mn<0?z:min(mn,z);mx=max(mx,z);
		}
		require(closest_pair2(a)==mn,"closest pair vs all pairs, duplicates/empty");
		require(hull_diameter2(h)==mx,"diameter vs all pairs");
		ld width=0;
		if(h.size()>=3){
			width=1e30L;
			for(int i=0;i<(int)h.size();i++){
				ld far=0;for(auto q:h) far=max(far,(ld)det(h[i],h[(i+1)%h.size()],q));
				width=min(width,far/sqrtl((ld)dist2(h[i],h[(i+1)%h.size()])));
			}
		}
		near(hull_min_width(h),width,1e-14L,"width vs all support vertices");
		for(int x=-2;x<=2;x++) for(int y=-2;y<=2;y++){
			IPoint q{x,y};int where=polygon_model(h,q);
			require(point_in_polygon(h,q)==where,"winding vs rational ray crossing");
			require(point_in_convex(h,q)==where,"fan search vs rational ray crossing");
		}
	}
	vector<IPoint> v;for(int x=-2;x<=2;x++) for(int y=-2;y<=2;y++) v.push_back({x,y});
	for(auto a:v){
		require(!polar_less(a,a),"polar irreflexive");
		for(auto b:v){
			require(!(polar_less(a,b)&&polar_less(b,a)),"polar antisymmetric");
			for(auto c:v) if(polar_less(a,b)&&polar_less(b,c)) require(polar_less(a,c),"polar transitive");
		}
	}
	for(int t=0;t<1000;t++){
		vector<IPoint> a,b;
		for(int i=rd(0,14);i;i--) a.push_back({rd(-20,20),rd(-20,20)});
		for(int i=rd(0,14);i;i--) b.push_back({rd(-20,20),rd(-20,20)});
		vector<IPoint> sums;for(auto p:a) for(auto q:b) sums.push_back(p+q);
		require(minkowski_sum(a,b)==model_hull(sums),"Minkowski vs all pairwise sums/gift wrapping");
		auto h=convex_hull(a);
		__int128 mn=-1,mx=a.empty()?-1:0;
		for(int i=0;i<(int)a.size();i++) for(int j=i+1;j<(int)a.size();j++){
			__int128 z=dist2(a[i],a[j]);mn=mn<0?z:min(mn,z);mx=max(mx,z);
		}
		require(closest_pair2(a)==mn&&hull_diameter2(h)==mx,"random distance pairs");
	}
	vector<IPoint> wide={{-1000000000000LL,-1000000000000LL},{1000000000000LL,-1000000000000LL},
		{1000000000000LL,1000000000000LL},{-1000000000000LL,1000000000000LL}};
	for(auto a:wide) for(auto b:wide) for(auto c:wide) require(I128(orient(a,b,c))==det(a,b,c),"128-bit promotion before multiply");
	I128 want=I128(8)*I128(1000000000000LL)*1000000000000LL;
	for(auto got:{hull_diameter2(wide),polygon_area2(wide)}) require(I128(got)==want,"wide diameter/area");
	for(int w=1;w<=6;w++) for(int h=1;h<=6;h++){
		vector<IPoint> p={{0,0},{w,0},{w,h},{0,h}};
		auto [in,bound]=lattice_points(p);require(in==(w-1)*(h-1)&&bound==2*(w+h),"Pick rectangle counts");
		reverse(p.begin(),p.end());require(lattice_points(p)==pair<__int128,__int128>{in,bound},"Pick orientation");
	}
	vector<IPoint> concave={{0,0},{4,0},{4,1},{1,1},{1,4},{0,4}};
	for(int x=-1;x<=5;x++) for(int y=-1;y<=5;y++) require(point_in_polygon(concave,{x,y})==polygon_model(concave,{x,y}),"concave boundary/horizontal vertex");
}
Dec decimal_overlap(Circle a,Circle b){
	Dec x=Dec(a.o.x)-b.o.x,y=Dec(a.o.y)-b.o.y,d=sqrt(x*x+y*y),r=a.r,s=b.r;
	Dec pi=acos(Dec(-1));
	if(d>=r+s) return 0;
	if(d<=abs(r-s)) return pi*min(r,s)*min(r,s);
	Dec u=(d*d+r*r-s*s)/(2*d*r),v=(d*d+s*s-r*r)/(2*d*s);
	Dec z=(-d+r+s)*(d+r-s)*(d-r+s)*(d+r+s);
	return r*r*acos(u)+s*s*acos(v)-sqrt(z)/2;
}
void real_checks(){
	for(int t=0;t<700;t++){
		Point a{(ld)rd(-20,20),(ld)rd(-20,20)},b{(ld)rd(-20,20),(ld)rd(-20,20)};
		if(length(a-b)==0) continue;
		Line l(a,b);Point p{(ld)rd(-20,20),(ld)rd(-20,20)};
		Point q=projection(p,l),r=reflection(p,l);
		near(l.side(q),0,1e-14L,"projection on line");
		near(dot(p-q,l.v),0,1e-13L,"projection orthogonal");
		near(l.side(r),-l.side(p),1e-13L,"reflection signed distance");
		near(point_segment_distance(p,a,a),distance(p,a),1e-14L,"degenerate segment distance");
		auto hit=line_intersection(l,Line(p,p+perp(l.v)));
		require(hit.kind==1,"perpendicular intersection kind");
		near(distance(hit.p,q),0,1e-12L,"intersection equals projection");
		Circle c{a,(ld)rd(0,10)},d{b,(ld)rd(0,10)};
		auto points=circle_line(c,l);
		require(points.size()==(c.r==0?1:2),"circle line through center");
		for(auto v:points){near(distance(v,c.o),c.r,1e-13L,"circle-line radius");near(l.side(v),0,1e-13L,"circle-line residual");}
		auto inter=circle_intersection(c,d);
		ld dist=distance(a,b);int want;
		if(c.r==0) want=fabsl(dist-d.r)<1e-12L;
		else if(d.r==0) want=fabsl(dist-c.r)<1e-12L;
		else if(dist>c.r+d.r+1e-12L||dist<fabsl(c.r-d.r)-1e-12L) want=0;
		else if(fabsl(dist-c.r-d.r)<1e-12L||fabsl(dist-fabsl(c.r-d.r))<1e-12L) want=1;
		else want=2;
		require(inter.cnt==want,"circle-circle classification");
		for(auto v:inter.p){near(distance(v,a),c.r,1e-12L,"circle-circle first radius");near(distance(v,b),d.r,1e-12L,"circle-circle second radius");}
		near(circle_overlap(c,d),decimal_overlap(c,d),2e-14L,"circle overlap vs independent sector formula");
		if(c.r>0&&d.r>0) for(bool inner:{false,true}){
			auto tang=circle_tangents(c,d,inner);ld need=inner?c.r+d.r:fabsl(c.r-d.r);
			int cnt=dist<need-1e-12L?0:(fabsl(dist-need)<1e-12L?1:2);
			require((int)tang.lines.size()==cnt&&!tang.infinite,"tangent count");
			for(auto v:tang.lines){
				near(distance(v.a,c.o),c.r,1e-12L,"first tangent contact");near(distance(v.b,d.o),d.r,1e-12L,"second tangent contact");
				near(dot(v.a-c.o,v.dir),0,1e-12L,"tangent perpendicular radius");near(cross(v.b-v.a,v.dir),0,1e-12L,"contacts share tangent");
			}
		}
	}
	Circle unit{{0,0},1};
	require(circle_intersection(unit,unit).cnt==-1,"coincident circle marker");
	require(circle_intersection(Circle{{0,0},0},Circle{{0,0},0}).cnt==1,"coincident point circles");
	require(circle_intersection(unit,Circle{{0,0},2}).cnt==0,"concentric unequal circles");
	require(circle_tangents(unit,unit).infinite&&circle_tangents(unit,unit,true).lines.empty(),"coincident tangent marker");
	require(tangent_points(Point{0,0},Circle{{0,0},1e-12L}).empty(),"tiny circle center no division by zero");
	for(ld delta:{-0.25e-10L,0.25e-10L}){
		Circle a{{0,0},1e6L},b{{0,0},1e6L+1e-6L};
		b.o={b.r-a.r+delta,0};
		auto hit=circle_intersection(a,b);
		require(hit.cnt==1,"near internal tangent tolerance class");
		for(auto p:hit.p){
			near(distance(p,a.o)-a.r,0,1.1e-10L,"near tangent point stays on first circle");
			near(distance(p,b.o)-b.r,0,1.1e-10L,"near tangent point stays on second circle");
		}
		auto tang=circle_tangents(a,b);
		require(tang.lines.size()==1,"near internal tangent single external line");
		for(auto t:tang.lines){
			near(distance(t.a,a.o)-a.r,0,1.1e-10L,"near tangent first contact radius");
			near(distance(t.b,b.o)-b.r,0,1.1e-10L,"near tangent second contact radius");
			near(length(t.dir),1,1e-15L,"near tangent normal remains unit length");
		}
	}
	for(Point p:vector<Point>{{0,0},{1,0},{3,4}}){
		auto v=tangent_points(p,unit);int cnt=length(p)<1?0:(length(p)==1?1:2);
		require((int)v.size()==cnt,"point tangent count");
		for(auto q:v){near(length(q),1,1e-14L,"point tangent radius");near(dot(q,p-q),0,1e-13L,"point tangent orthogonality");}
	}
	for(ld off:{0.0L,1e9L}){
		Point a{off,off},b{off+4,off},c{off,off+3};
		auto out=circumcircle(a,b,c),in=incircle(a,b,c);
		require(out.has_value()&&in.has_value(),"triangle circles nondegenerate");
		near(out->r,2.5L,1e-12L,"right triangle circumradius");near(in->r,1,1e-12L,"right triangle inradius");
		near(distance(out->o,Point{off+2,off+1.5L}),0,1e-9L,"translated circumcenter");
		vector<Point> poly={{off,off},{off+4,off},{off+4,off+3},{off,off+3}};
		near(polygon_area2(poly),24,1e-15L,"translated area");
		auto center=polygon_centroid(poly);require(center.has_value(),"centroid exists");near(distance(*center,Point{off+2,off+1.5L}),0,1e-9L,"translated centroid");
	}
	require(!circumcircle(Point{0,0},Point{1,0},Point{2,0}),"collinear circumcircle undefined");
	require(!incircle(Point{0,0},Point{0,0},Point{2,0}),"repeated incircle undefined");
}
struct HP{long long a,b,c;};
Line hp_line(HP h){
	Point p=h.a?Point{-(ld)h.c/h.a,0}:Point{0,-(ld)h.c/h.b};
	return Line(p,p+Point{(ld)h.b,-(ld)h.a});
}
void halfplane_checks(){
	for(int t=0;t<1800;t++){
		vector<Point> slow={{-20,-20},{20,-20},{20,20},{-20,20}};
		vector<Line> lines;vector<HP> hp;
		for(int n=rd(0,14);n;n--){
			HP h{rd(-3,3),rd(-3,3),rd(-10,10)};if(h.a==0&&h.b==0){n++;continue;}
			hp.push_back(h);lines.push_back(hp_line(h));slow=cut_polygon(slow,lines.back());
		}
		ld want=fabsl(polygon_area2(slow));
		auto fast=halfplane_intersection(lines,20);
		if(fabsl(fabsl(polygon_area2(fast))-want)>2e-11L*(1+want)){
			cerr<<"HPI integer case "<<t<<'\n';for(auto h:hp) cerr<<h.a<<' '<<h.b<<' '<<h.c<<'\n';
		}
		near(fabsl(polygon_area2(fast)),want,2e-11L,"deque HPI vs sequential clipping");
		require((want<=1e-16L)==fast.empty(),"HPI positive-area contract");
		for(auto p:fast) for(auto l:lines) require(l.side(p)>=-1e-8L,"HPI all constraints");
	}
	for(ld width:{0.0L,1e-6L,1.0L}){
		vector<Line> lines={hp_line({0,1,0}),hp_line({0,-1,1}),hp_line({1,0,0})};
		lines.emplace_back(Point{width,0},Point{width,1});
		auto p=halfplane_intersection(lines,5);
		near(fabsl(polygon_area2(p))/2,width,1e-12L,"parallel strip/zero-area HPI");
	}
	vector<Point> p={{0,0},{2,0},{2,2},{0,2}};
	auto point=cut_polygon(cut_polygon(p,Line(Point{0,0},Point{-1,0})),Line(Point{0,0},Point{0,1}));
	require(point.size()==1&&same_point(point[0],Point{0,0}),"clip retains point intersection");
	vector<Point> q={{1,1},{3,1},{3,3},{1,3}};
	near(fabsl(polygon_area2(convex_intersection(p,q)))/2,1,1e-14L,"convex polygon overlap");
	reverse(q.begin(),q.end());near(fabsl(polygon_area2(convex_intersection(p,q)))/2,1,1e-14L,"clockwise clip polygon");
	vector<Point> thin={{-2e-10L,0},{-0.5e-10L,0},{-0.5e-10L,1},{-2e-10L,1}};
	auto v=cut_polygon(thin,Line(Point{0,1},Point{0,0}));
	for(auto a:v) require(a.x>=-2e-10L&&a.x<=-0.5e-10L,"EPS clipping never extrapolates outside edge");
}
struct DCircle{Dec x,y,r2;};
ld mec_model(const vector<Point>& p){
	if(p.empty()) return -1;
	vector<DCircle> candidates;
	for(auto a:p) candidates.push_back({Dec(a.x),Dec(a.y),0});
	for(int i=0;i<(int)p.size();i++) for(int j=0;j<i;j++){
		Dec x=(Dec(p[i].x)+p[j].x)/2,y=(Dec(p[i].y)+p[j].y)/2;
		Dec dx=x-p[i].x,dy=y-p[i].y;candidates.push_back({x,y,dx*dx+dy*dy});
		for(int k=0;k<j;k++){
			Dec ux=Dec(p[j].x)-p[i].x,uy=Dec(p[j].y)-p[i].y,vx=Dec(p[k].x)-p[i].x,vy=Dec(p[k].y)-p[i].y;
			Dec d=2*(ux*vy-uy*vx);if(d==0) continue;
			Dec x=((ux*ux+uy*uy)*vy-(vx*vx+vy*vy)*uy)/d;
			Dec y=(ux*(vx*vx+vy*vy)-vx*(ux*ux+uy*uy))/d;
			candidates.push_back({x+p[i].x,y+p[i].y,x*x+y*y});
		}
	}
	Dec best=1e100L;
	for(auto c:candidates){
		bool ok=true;for(auto a:p){Dec dx=Dec(a.x)-c.x,dy=Dec(a.y)-c.y;if(dx*dx+dy*dy>c.r2+1e-14L) ok=false;}
		if(ok&&c.r2<best) best=c.r2;
	}
	return sqrtl(best);
}
ld adaptive_integral(const function<ld(ld)>& f,ld l,ld r,ld eps,int depth=22){
	ld mid=(l+r)/2,a=f(l),b=f(mid),c=f(r),whole=(r-l)*(a+4*b+c)/6;
	auto go=[&](auto&& self,ld l,ld r,ld a,ld b,ld c,ld whole,ld eps,int depth)->ld{
		ld m=(l+r)/2,u=f((l+m)/2),v=f((m+r)/2);
		ld x=(m-l)*(a+4*u+b)/6,y=(r-m)*(b+4*v+c)/6;
		if(depth==0||fabsl(x+y-whole)<=15*eps) return x+y+(x+y-whole)/15;
		return self(self,l,m,a,u,b,x,eps/2,depth-1)+self(self,m,r,b,v,c,y,eps/2,depth-1);
	};
	return go(go,l,r,a,b,c,whole,eps,depth);
}
ld integration_model(Circle c,const vector<Point>& p){
	vector<ld> xs={c.o.x-c.r,c.o.x+c.r};
	for(auto a:p) if(a.x>xs[0]&&a.x<xs[1]) xs.push_back(a.x);
	sort(xs.begin(),xs.end());
	auto height=[&](ld x){
		ld h=sqrtl(max(0.0L,c.r*c.r-(x-c.o.x)*(x-c.o.x))),sum=0;
		vector<ld> ys;
		for(int i=0;i<(int)p.size();i++){
			auto a=p[i],b=p[(i+1)%p.size()];if(a.x>b.x) swap(a,b);
			if(a.x<=x&&x<b.x) ys.push_back(a.y+(b.y-a.y)*(x-a.x)/(b.x-a.x));
		}
		sort(ys.begin(),ys.end());
		for(int i=0;i+1<(int)ys.size();i+=2) sum+=max(0.0L,min(ys[i+1],c.o.y+h)-max(ys[i],c.o.y-h));
		return sum;
	};
	ld ans=0;
	for(int i=1;i<(int)xs.size();i++) if(xs[i]>xs[i-1]){
		ld l=xs[i-1],r=xs[i];
		// Subdivide to avoid missing narrow nonzero overlap between sample points.
		for(int j=0;j<32;j++) ans+=adaptive_integral(height,l+(r-l)*j/32,l+(r-l)*(j+1)/32,1e-10L);
	}
	return ans;
}
void circle_area_mec_checks(){
	vector<vector<Point>> sets={{},{{1,2}},{{0,0},{0,0}},{{-3,0},{2,0},{0,0}},
		{{0,0},{4,0},{0,3}},{{0,0},{1,1e-12L},{2,0}},{{1e9L,1e9L},{1e9L+4,1e9L},{1e9L,1e9L+3}}};
	for(int t=0;t<240;t++){
		vector<Point> p;for(int n=rd(0,9);n;n--) p.push_back({(ld)rd(-10,10),(ld)rd(-10,10)});sets.push_back(p);
	}
	for(auto p:sets){
		ld want=mec_model(p);
		for(uint64_t seed:{0ULL,1ULL,123456ULL,987654321ULL}){
			auto c=minimum_circle(p,seed);near(c.r,want,2e-10L,"MEC vs exhaustive support enumeration");
			for(auto a:p) require(circle_contains(c,a),"MEC encloses every input under documented tolerance");
		}
	}
	Circle c{{0,0},1};
	vector<Point> square={{-2,-2},{2,-2},{2,2},{-2,2}},quarter={{0,0},{2,0},{2,2},{0,2}};
	near(circle_polygon_area(c,square),GEO_PI,1e-14L,"circle fully in polygon");
	near(circle_polygon_area(c,quarter),GEO_PI/4,1e-14L,"quarter disk incl center/boundary");
	near(circle_polygon_area(Circle{{0,0},10},quarter),4,1e-14L,"polygon fully in disk");
	quarter.push_back(quarter.back());reverse(quarter.begin(),quarter.end());
	near(circle_polygon_area(c,quarter),GEO_PI/4,1e-14L,"circle polygon CW/repeated endpoints");
	for(int t=0;t<35;t++){
		vector<IPoint> pts;for(int n=8;n;n--) pts.push_back({rd(-5,5),rd(-5,5)});
		auto h=convex_hull(pts);vector<Point> p;for(auto a:h) p.push_back({(ld)a.x,(ld)a.y});
		if(p.size()<3) continue;
		Circle disk{Point{(ld)rd(-3,3),(ld)rd(-3,3)},(ld)rd(1,5)};
		near(circle_polygon_area(disk,p),integration_model(disk,p),8e-7L,"circle polygon vs independent vertical integration");
	}
	vector<Point> concave={{-3,-3},{3,-3},{3,-1},{-1,-1},{-1,3},{-3,3}};
	near(circle_polygon_area(Circle{{-1,-1},2},concave),integration_model(Circle{{-1,-1},2},concave),8e-7L,"concave circle polygon integration");
}
void circle_polygon_tangent_checks(){
	// Independent analytic area of the part of a disk on x<=cut.
	auto cap_area=[](ld cut,ld r){
		if(cut<=-r) return 0.0L;
		if(cut>=r) return GEO_PI*r*r;
		ld t=cut/r;
		return r*r*(asinl(t)+GEO_PI/2+t*sqrtl((1-t)*(1+t)));
	};
	for(ld r:{1e-6L,1e-3L,1.0L,1e3L}) for(ld far:{2.0L,1e3L,1e6L}){
		ld B=max(2*r,far);
		for(ld t:{-1.000001L,-1.0L,-0.999999L,-0.9L,-0.1L,0.0L,0.1L,0.9L,0.999999L,1.0L,1.000001L}){
			ld x=t*r,want=cap_area(x,r);
			vector<Point> p={{-B,-B},{x,-B},{x,B},{-B,B}};
			for(int rot=0;rot<4;rot++){
				for(int orientation=0;orientation<2;orientation++){
					ld got=circle_polygon_area({{0,0},r},p);
					require(isfinite(got)&&fabsl(got-want)<=2e-9L*r*r+1e-19L,"circle-polygon analytic disk cap, tangency and crossing");
					reverse(p.begin(),p.end());
				}
				for(auto& v:p) v=perp(v);
			}
		}
	}
	// Arbitrary angles expose rounding on both sides of tangency; the projection
	// must be a split even if the computed squared intersection height is negative.
	for(ld r:{1e-6L,1.0L,1e3L}) for(ld len:{2.0L,1e3L}){
		ld B=max(len,2*r);
		for(int i=0;i<720;i++){
			ld theta=i*GEO_PI/360;
			Point normal={cosl(theta),sinl(theta)},along=perp(normal);
			Point a=normal*r-along*B,b=normal*r+along*B;
			for(int side:{-1,1}){
				vector<Point> p={a,b,b+normal*(side*2*B),a+normal*(side*2*B)};
				ld want=side==1?0:GEO_PI*r*r;
				for(int orientation=0;orientation<2;orientation++){
					ld got=circle_polygon_area({{0,0},r},p);
					require(isfinite(got)&&fabsl(got-want)<=1e-8L*r*r+1e-19L,"circle-polygon rotated tangent rectangle, inside and outside");
					reverse(p.begin(),p.end());
				}
			}
		}
	}
}
void space_checks(){
	Point3 a{0,0,0},b{1,0,0},c{0,1,0},d{0,0,1};
	vector<array<Point3,3>> faces={{a,c,b},{a,b,d},{a,d,c},{b,c,d}};
	near(polyhedron_signed_volume(faces),1.0L/6,1e-15L,"tetra oriented surface volume");
	for(auto& t:faces) for(auto& p:t) p=p+Point3{1e9L,-1e9L,2e9L};
	near(polyhedron_signed_volume(faces),1.0L/6,1e-15L,"translated oriented volume");
	for(auto& t:faces) swap(t[1],t[2]);
	near(polyhedron_signed_volume(faces),-1.0L/6,1e-15L,"reversed oriented volume");
	near(tetrahedron_volume(a,b,c,d),1.0L/6,1e-15L,"tetra volume");
	Plane f(Point3{0,0,2},Point3{0,0,3});
	near(f.side(Point3{0,0,3}),1,1e-15L,"plane normalized distance");
	auto hit=line_plane(a,Point3{2,2,4},f);require(hit.kind==1,"line plane unique");near(f.side(hit.p),0,1e-15L,"line plane residual");
	require(line_plane(a,b,f).kind==0&&line_plane(Point3{0,0,2},Point3{1,0,2},f).kind==2,"parallel/coincident line plane");
	near(space_line_distance(a,b,Point3{0,0,3},Point3{0,1,3}),3,1e-15L,"skew lines distance");
	near(space_line_distance(a,b,Point3{0,2,0},Point3{1,2,0}),2,1e-15L,"parallel lines distance");
	near(space_line_distance(a,a,b,b),1,1e-15L,"two point-lines distance");
	near(great_circle_distance(0,0,0,GEO_PI/2,2),GEO_PI,1e-15L,"quarter great circle");
	near(great_circle_distance(0,0,0,GEO_PI,1),GEO_PI,1e-15L,"antipodal sphere");
	near(great_circle_distance(0,0,0,1e-12L,1),1e-12L,1e-18L,"tiny spherical angle");
}

void halfplane_wrap_checks(){
	mt19937_64 order(20261015);
	for(ld B:{1e-6L,1.0L,1e6L}) for(ld slope:{-1e-14L,-5e-18L,-2e-18L,-1.1e-18L,-0.9e-18L,-5e-19L,0.0L,5e-19L,0.9e-18L,1.1e-18L,2e-18L,5e-18L,1e-14L}){
		for(ld ratio:{0.5L,1.0L}) for(ld shift:{-GEO_EPS/4,0.0L,GEO_EPS/4}) for(int rot=0;rot<4;rot++){
			auto turn=[&](Point p){for(int k=0;k<rot;k++) p={-p.y,p.x};return p;};
			Line line(turn(Point{-B,-B*ratio+shift}),turn(Point{B,-B*ratio+shift+2*B*slope}));
			for(int mode=0;mode<3;mode++){
				vector<Line> a{line};
				if(mode>=1){
					a.push_back(line);
					a.emplace_back(turn(Point{-B,-B*ratio}),turn(Point{B,-B*ratio}));
				}
				if(mode==2){
					for(int k=1;k<4;k++){
						Point p=line.p,v=line.v;
						for(int j=0;j<k;j++){p={-p.y,p.x};v={-v.y,v.x};}
						a.emplace_back(p,p+v*(2*B));
					}
				}
				vector<Point> ref={{-B,-B},{B,-B},{B,B},{-B,B}};
				// Independent edge-by-edge clipping without widening by GEO_EPS.
				for(auto l:a){
					vector<Point> next;
					for(int i=0;i<(int)ref.size();i++){
						Point p=ref[i],q=ref[(i+1)%ref.size()];
						ld x=l.side(p),y=l.side(q);
						if(x>=0) next.push_back(p);
						if((x>=0)!=(y>=0)) next.push_back(p+(q-p)*(x/(x-y)));
					}
					ref=next;
				}
				ld want=fabsl(polygon_area2(ref))/2;
				require(want>0,"HPI wrap reference has positive area");
				for(int repeat=0;repeat<2;repeat++){
					shuffle(a.begin(),a.end(),order);
					auto got=halfplane_intersection(a,B);
					if(got.size()<3) cerr<<setprecision(24)<<"HPI wrap B="<<B<<" slope="<<slope<<" shift="<<shift<<" rot="<<rot<<" mode="<<mode<<" repeat="<<repeat<<'\n';
					require(got.size()>=3,"HPI wrap must not discard positive region");
					ld area=fabsl(polygon_area2(got))/2;
					require(fabsl(area-want)<=max(1e-24L,B*B*1e-8L),"HPI wrap vs sequential clipping");
				}
			}
		}
	}
}

void geometry_transform_checks(){
	// Exact integer answers survive large translations, integer scaling and rotations.
	for(int test=0;test<100;test++){
		vector<IPoint> base;
		for(int i=0;i<25;i++) base.push_back({rd(-30,30),rd(-30,30)});
		auto h=convex_hull(base);I128 dia=0,closest=(I128)1<<120;
		for(int i=0;i<(int)base.size();i++) for(int j=0;j<i;j++){
			dia=max(dia,dist2(base[i],base[j]));closest=min(closest,dist2(base[i],base[j]));
		}
		ld width=hull_min_width(h);
		for(long long scale:{1LL,1000000000LL}) for(int turn=0;turn<4;turn++){
			auto change=[&](IPoint a){
				for(int j=0;j<turn;j++) a={-a.y,a.x};
				return IPoint{a.x*scale+900000000000LL,a.y*scale-900000000000LL};
			};
			vector<IPoint> p;for(auto a:base) p.push_back(change(a));
			auto q=convex_hull(p);
			require(closest_pair2(p)==closest*scale*scale,"large integer closest exact");
			require(hull_diameter2(q)==dia*scale*scale,"large integer diameter exact");
			near(hull_min_width(q),width*scale,1e-15L,"large integer width scales");
			for(int j=0;j<50;j++){
				IPoint a{rd(-40,40),rd(-40,40)};
				require(point_in_convex(q,change(a))==point_in_polygon(h,a),"large integer fan vs winding");
			}
		}
	}
	// Analytic rectangle area, including aspect ratio 1e12; all constraints shuffled.
	for(ld len:{1e-6L,1.0L,1e6L}) for(ld width:{1e-6L,1.0L}){
		if(width>len) continue;
		for(int angle=0;angle<180;angle++){
			ld theta=angle*GEO_PI/180;Point u{cosl(theta),sinl(theta)},v=perp(u);
			vector<Point> p={u*(-len)+v*(-width),u*len+v*(-width),u*len+v*width,u*(-len)+v*width};
			vector<Line> a;
			for(int i=0;i<4;i++) a.emplace_back(p[i],p[(i+1)%4]);
			for(int rep=0;rep<3;rep++){
				shuffle(a.begin(),a.end(),rng);auto q=halfplane_intersection(a,len*3);
				ld got=fabsl(polygon_area2(q))/2,want=4*len*width;
				require(!q.empty()&&fabsl(got-want)<=want*2e-6L,"rotated thin HPI rectangle analytic area");
			}
		}
	}
	for(ld scale:{1e-6L,1.0L,1e6L}) for(int t=0;t<500;t++){
		ld ang=(ld)rd(0,100000)*GEO_PI/50000;
		Point u{cosl(ang),sinl(ang)},v=perp(u);
		for(bool inner:{false,true}){
			Circle a{{0,0},scale},b{u*(inner?2*scale:scale),scale/2};
			auto h=circle_tangents(a,b,inner);
			require(h.lines.size()==2,"scaled inner/outer tangent count");
			for(auto z:h.lines){
				require(fabsl(distance(z.a,a.o)-a.r)<=4e-10L,"scaled tangent radius a");
				require(fabsl(distance(z.b,b.o)-b.r)<=4e-10L,"scaled tangent radius b");
				require(fabsl(cross(z.b-z.a,z.dir))<=4e-10L,"scaled tangent contact line");
			}
		}
		auto outer=circumcircle(Point{0,0},u*scale,v*scale);
		require(outer.has_value(),"scaled triangle circle exists");
		near(outer->r,scale/sqrtl(2),1e-14L,"rotated triangle circumradius");
	}
}
void space_conditioning_checks(){
	for(int t=0;t<700;t++){
		Point3 a{(ld)rd(-100000,100000),(ld)rd(-100000,100000),(ld)rd(-100000,100000)};
		Point3 b{(ld)rd(-100000,100000),(ld)rd(-100000,100000),(ld)rd(-100000,100000)};
		Point3 c{(ld)rd(-100000,100000),(ld)rd(-100000,100000),(ld)rd(-100000,100000)};
		I128 ax=a.x,ay=a.y,az=a.z,bx=b.x,by=b.y,bz=b.z,cx=c.x,cy=c.y,cz=c.z;
		I128 exact=ax*(by*cz-bz*cy)+ay*(bz*cx-bx*cz)+az*(bx*cy-by*cx);
		require(triple(a,b,c)==(ld)exact,"3D triple product vs exact integer determinant");
		Point3 n{(ld)rd(-10,10),(ld)rd(-10,10),(ld)rd(-10,10)};
		Point3 v{(ld)rd(-10,10),(ld)rd(-10,10),(ld)rd(-10,10)};
		if(length(n)==0||length(v)==0||dot(n,v)==0) continue;
		Plane f(a,n);
		Point3 p=projection(b,f);
		near(f.side(p),0,2e-9L,"3D projection on arbitrary plane");
		near(length(cross(b-p,f.n)),0,2e-9L,"3D projection normal direction");
		for(ld scale:{1e-6L,1.0L,1e3L}){
			auto hit=line_plane(a+v*(2*scale),a-v*(3*scale),f);
			require(hit.kind==1,"3D known line-plane intersection exists");
			near(length(hit.p-a),0,2e-8L,"3D line-plane known intersection at varied scales");
			Point3 normal=cross(n,v);ld z=length(normal);
			if(z==0) continue;
			Point3 u=a+normal*(scale/z);
			near(space_line_distance(a,a+n,u,u+v),scale,2e-9L,"3D skew-line analytic separation");
		}
	}
}
void oracle_protocol(){
	int type;
	while(cin>>type){
		if(type==0){
			IPoint a,b,c;cin>>a.x>>a.y>>b.x>>b.y>>c.x>>c.y;
			__int128 v=orient(a,b,c);bool neg=v<0;if(neg) v=-v;
			string s;do{s.push_back('0'+v%10);v/=10;}while(v);if(neg) s.push_back('-');reverse(s.begin(),s.end());cout<<s<<'\n';
		}
		else if(type==1){
			Circle a,b;cin>>a.o.x>>a.o.y>>a.r>>b.o.x>>b.o.y>>b.r;cout<<circle_overlap(a,b)<<'\n';
		}
		else if(type==2){
			int n;cin>>n;vector<Point> p(n);for(auto& q:p) cin>>q.x>>q.y;
			auto c=minimum_circle(p,1234567);cout<<c.o.x<<' '<<c.o.y<<' '<<c.r<<'\n';
		}
		else if(type==3||type==5){
			int n;ld B;cin>>n>>B;vector<Line> lines;
			while(n--){HP h;cin>>h.a>>h.b>>h.c;lines.push_back(hp_line(h));}
			vector<Point> p;
			if(type==3) p=halfplane_intersection(lines,B);
			else{
				p={{-B,-B},{B,-B},{B,B},{-B,B}};
				for(auto l:lines) p=cut_polygon(p,l);
			}
			cout<<fabsl(polygon_area2(p))/2<<'\n';
		}
		else if(type==4){
			Circle a,b;cin>>a.o.x>>a.o.y>>a.r>>b.o.x>>b.o.y>>b.r;
			auto hit=circle_intersection(a,b);cout<<hit.cnt;
			for(auto p:hit.p) cout<<' '<<p.x<<' '<<p.y;
			cout<<'\n';
		}
	}
}
signed main(signed argc,char**){
	cout<<setprecision(25);
	if(argc>1){oracle_protocol();return 0;}
	integer_checks();real_checks();halfplane_checks();circle_area_mec_checks();space_checks();halfplane_wrap_checks();circle_polygon_tangent_checks();
	geometry_transform_checks();space_conditioning_checks();
	cout<<"PASS: geometry, "<<checks<<" independent-model/adversarial checks; long double digits="<<numeric_limits<ld>::digits<<'\n';
}
