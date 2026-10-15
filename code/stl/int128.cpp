bool read128(__int128 &x){
	string s;if(!(cin>>s)) return false;
	int p=0;bool neg=s[0]=='-';
	if(s[0]=='+'||s[0]=='-') p=1;
	x=0;
	for(;p<(int)s.size();p++){
		int d=s[p]-'0';
		if(neg) x=x*10-d;
		else x=x*10+d;
	}
	return true;
}
string str128(__int128 x){
	if(!x) return "0";
	bool neg=x<0;
	__uint128_t v=neg?-(__uint128_t)x:(__uint128_t)x;
	string s;
	while(v){s.push_back('0'+v%10);v/=10;}
	if(neg) s.push_back('-');
	reverse(s.begin(),s.end());return s;
}
