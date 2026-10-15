// Dependencies: point.cpp, circle.cpp, triangle.cpp. Randomized incremental MEC.
// 含边界的圆盘包含判断；半径为负的无效圆返回 false。
bool circle_contains(const Circle& c,Point p){return c.r>=0&&distance(c.o,p)<=c.r+GEO_EPS;}
// 以 ab 为直径构造圆；a=b 时返回点圆。
Circle diameter_circle(Point a,Point b){Point o=a+(b-a)/2;return {o,distance(o,a)};}
// 随机增量最小覆盖圆；空集返回 r=-1，seed 可固定以复现。
Circle minimum_circle(vector<Point> p,uint64_t seed=random_device{}()){
	if(p.empty()) return {{},-1};
	shuffle(p.begin(),p.end(),mt19937_64(seed));
	Circle c{p[0],0};
	for(int i=0;i<(int)p.size();i++) if(!circle_contains(c,p[i])){
		c={p[i],0};
		for(int j=0;j<i;j++) if(!circle_contains(c,p[j])){
			c=diameter_circle(p[i],p[j]);
			for(int k=0;k<j;k++) if(!circle_contains(c,p[k])){
				auto cur=circumcircle(p[i],p[j],p[k]);
				if(cur) c=*cur;
				else{
					c=diameter_circle(p[i],p[j]);
					for(auto [a,b]:{pair<Point,Point>{p[i],p[k]},{p[j],p[k]}}){
						auto d=diameter_circle(a,b);if(d.r>c.r) c=d;
					}
				}
			}
		}
	}
	return c;
}
