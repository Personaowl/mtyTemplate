// Dependencies: integer_point.cpp, point.cpp. Strict CCW hull, no repeated endpoint.
// 旋转卡壳求严格逆时针凸包的直径平方；空集 -1，单点 0。
__int128 hull_diameter2(const vector<IPoint>& p){
	int n=p.size();if(n==0) return -1;
	if(n<=2) return dist2(p[0],p[n-1]);
	int j=1;__int128 ans=0;
	for(int i=0;i<n;i++){
		int k=(i+1)%n;
		while(orient(p[i],p[k],p[(j+1)%n])>orient(p[i],p[k],p[j])) j=(j+1)%n;
		ans=max({ans,dist2(p[i],p[j]),dist2(p[k],p[j])});
		int t=(j+1)%n;
		if(orient(p[i],p[k],p[t])==orient(p[i],p[k],p[j])){
			ans=max({ans,dist2(p[i],p[t]),dist2(p[k],p[t])});
		}
	}
	return ans;
}
// 旋转卡壳求包住凸包的最窄平行带宽度；至多两点返回 0。
ld hull_min_width(const vector<IPoint>& p){
	int n=p.size();if(n<=2) return 0;
	int j=1;ld ans=numeric_limits<ld>::infinity();
	for(int i=0;i<n;i++){
		int k=(i+1)%n;
		while(orient(p[i],p[k],p[(j+1)%n])>orient(p[i],p[k],p[j])) j=(j+1)%n;
		ans=min(ans,(ld)orient(p[i],p[k],p[j])/sqrtl((ld)dist2(p[i],p[k])));
	}
	return ans;
}
