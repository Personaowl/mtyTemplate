// Dependency: integer_point.cpp. Exact squared distance; fewer than two points => -1.
// 分治求全部点中的最近点对距离平方；不足两点 -1，重复点 0。
__int128 closest_pair2(vector<IPoint> a){
	int n=a.size();if(n<2) return -1;
	sort(a.begin(),a.end());
	for(int i=1;i<n;i++) if(a[i]==a[i-1]) return 0;
	vector<IPoint> tmp(n);
	// 辅助排序：先 y 后 x，合并时保持纵坐标有序。
	auto by_y=[](IPoint a,IPoint b){return tie(a.y,a.x)<tie(b.y,b.x);};
	// 处理半开区间 [l,r)，返回最近距离平方，并将该段按 y 排序。
	auto dfs=[&](auto&& self,int l,int r)->__int128{
		if(r-l<=3){
			__int128 ans=(__int128)1<<120;
			for(int i=l;i<r;i++) for(int j=i+1;j<r;j++) ans=min(ans,dist2(a[i],a[j]));
			sort(a.begin()+l,a.begin()+r,by_y);return ans;
		}
		int mid=(l+r)>>1;long long x=a[mid].x;
		__int128 left=self(self,l,mid),right=self(self,mid,r),ans=min(left,right);
		merge(a.begin()+l,a.begin()+mid,a.begin()+mid,a.begin()+r,tmp.begin()+l,by_y);
		copy(tmp.begin()+l,tmp.begin()+r,a.begin()+l);
		vector<IPoint> strip;
		for(int i=l;i<r;i++){
			__int128 dx=(__int128)a[i].x-x;
			if(dx*dx>=ans) continue;
			for(int j=(int)strip.size()-1;j>=0;j--){
				__int128 dy=(__int128)a[i].y-strip[j].y;
				if(dy*dy>=ans) break;
				ans=min(ans,dist2(a[i],strip[j]));
			}
			strip.push_back(a[i]);
		}
		return ans;
	};
	return dfs(dfs,0,n);
}
