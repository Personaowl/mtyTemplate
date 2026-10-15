// 单谷函数最小值的位置；允许谷底为一段平区间。
template<class F>
long double ternary_min(long double l,long double r,F calc){
	for(int i=0;i<200;i++){
		long double a=l+(r-l)/3,b=r-(r-l)/3;
		if(a==l&&b==r) break;
		if(calc(a)<=calc(b)) r=b;
		else l=a;
	}
	return l+(r-l)/2;
}
// 整数域单谷，区间较短后逐点枚举；并列最优返回其中一个位置。
template<class F>
long long ternary_min_int(long long l,long long r,F calc){
	while(r-l>3){
		long long a=l+(r-l)/3,b=r-(r-l)/3;
		if(calc(a)<=calc(b)) r=b;
		else l=a;
	}
	long long ans=l;auto best=calc(l);
	for(long long x=l;x<r;x++){
		auto v=calc(x+1);
		if(v<best){best=v;ans=x+1;}
	}
	return ans;
}
