// Dependency: point3.cpp. Consistently outward-oriented triangular closed surface.
// 一致定向的闭合三角面网格有向体积；全部朝外为正，空面集为 0。
ld polyhedron_signed_volume(const vector<array<Point3,3>>& faces){
	if(faces.empty()) return 0;
	Point3 o=faces[0][0];ld sum=0,err=0;
	for(auto t:faces){
		ld val=triple(t[0]-o,t[1]-o,t[2]-o)/6-err,s=sum+val;
		err=(s-sum)-val;sum=s;
	}
	return sum;
}
// 四点构成的四面体非负体积。
ld tetrahedron_volume(Point3 a,Point3 b,Point3 c,Point3 d){
	return fabsl(triple(b-a,c-a,d-a))/6;
}
