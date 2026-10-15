// 闭区间内谓词先 false 后 true，要求 check(l)=false、check(r)=true。
template<class F>
long double first_true_real(long double l,long double r,F check){
	for(int i=0;i<200;i++){
		long double mid=l+(r-l)/2;
		if(mid==l||mid==r) break;
		if(check(mid)) r=mid;
		else l=mid;
	}
	return r;
}
// 闭区间内谓词先 true 后 false，要求 check(l)=true、check(r)=false。
template<class F>
long double last_true_real(long double l,long double r,F check){
	for(int i=0;i<200;i++){
		long double mid=l+(r-l)/2;
		if(mid==l||mid==r) break;
		if(check(mid)) l=mid;
		else r=mid;
	}
	return l;
}
