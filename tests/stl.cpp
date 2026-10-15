#include<bits/stdc++.h>
#ifdef MTY_INT64
#define int long long
#endif
using namespace std;
#include "code/stl/int128.cpp"

long long checks=0;
void require(bool ok,const char *name){
	checks++;if(!ok){cerr<<"FAIL: "<<name<<'\n';exit(1);}
}
signed main(){
	vector<string> input={"0","-0","+0","000123","-000123","+9876543210123456789",
		"170141183460469231731687303715884105727",
		"-170141183460469231731687303715884105728"};
	vector<string> expected={"0","0","0","123","-123","9876543210123456789",
		input[6],input[7]};
	string data;for(auto &s:input) data+=s+' ';
	istringstream stream(data);auto old=cin.rdbuf(stream.rdbuf());
	__int128 x=0;
	const __int128 hi=(__int128)(((__uint128_t)1<<127)-1),lo=-hi-1;
	for(int i=0;i<(int)input.size();i++){
		require(read128(x),"read valid decimal token");
		require(str128(x)==expected[i],"canonical decimal formatting");
		if(i==6) require(x==hi,"read largest signed 128-bit value");
		if(i==7) require(x==lo,"read smallest signed 128-bit value");
	}
	require(!read128(x)&&x==lo,"EOF returns false and preserves destination");
	cin.rdbuf(old);cin.clear();
	require(str128(hi)==input[6]&&str128(lo)==input[7],"format both signed extremes");
	require(str128((__int128)1000000000000LL*1000000000000LL)=="1000000000000000000000000",
		"wide multiplication before formatting");
	cout<<"PASS: STL 128-bit I/O checks ("<<checks<<")\n";
}
