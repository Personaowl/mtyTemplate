// Dependencies: point.cpp, line.cpp, polygon_real.cpp, polygon_cut.cpp.
// 求闭左半平面与 [-B,B]^2 的正面积交集；退化或空区域返回空。
vector<Point> halfplane_intersection(vector<Line> a,ld B){
	vector<Point> box={{-B,-B},{B,-B},{B,B},{-B,B}};
	for(int i=0;i<4;i++) a.emplace_back(box[i],box[(i+1)%4]);
	// 方向角映射到 [0,2pi)，用于严格排序。
	auto ang=[](const Line& l){ld v=atan2l(l.v.y,l.v.x);return v<0?v+2*GEO_PI:v;};
	sort(a.begin(),a.end(),[&](const Line& x,const Line& y){return ang(x)<ang(y);});
	// 单位方向的叉积判平行，使用角度阈值。
	auto parallel=[](const Line& x,const Line& y){return fabsl(cross(x.v,y.v))<=GEO_ANG;};
	// 判断点是否落在保留侧之外，含长度容差。
	auto out=[](const Line& l,Point p){return l.side(p)<-GEO_EPS;};
	// 取队列中相邻非平行直线的交点；只在不变量保证唯一时调用。
	auto inter=[](const Line& x,const Line& y){return line_intersection(x,y).p;};
	deque<Line> q;
	for(auto l:a){
		while(q.size()>1&&out(l,inter(q[q.size()-2],q.back()))) q.pop_back();
		while(q.size()>1&&out(l,inter(q[0],q[1]))) q.pop_front();
		if(!q.empty()&&parallel(q.back(),l)){
			if(dot(q.back().v,l.v)<0) return {};
			if(q.back().side(l.p)>0) q.pop_back();
			else continue;
		}
		q.push_back(l);
	}
	while(q.size()>2&&out(q.front(),inter(q[q.size()-2],q.back()))) q.pop_back();
	while(q.size()>2&&out(q.back(),inter(q[0],q[1]))) q.pop_front();
	if(q.size()<3||parallel(q.front(),q.back())) return {};
	vector<Point> p;
	for(int i=0;i<(int)q.size();i++) p.push_back(inter(q[i],q[(i+1)%q.size()]));
	p=clean_polygon(p);
	ld perimeter=0;
	for(int i=0;i<(int)p.size();i++) perimeter+=distance(p[i],p[(i+1)%p.size()]);
	if(p.size()<3||fabsl(polygon_area2(p))<=GEO_EPS*perimeter) return {};
	return p;
}
