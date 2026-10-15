#include<bits/stdc++.h>
#ifdef MTY_INT64
#define int long long
#endif
using namespace std;
#include "code/strings/hash_single.cpp"
#include "code/strings/hash_overflow.cpp"

long long checks=0;
void require(bool ok,const char *name){
	checks++;if(!ok){cerr<<"FAIL: "<<name<<'\n';exit(1);}
}
signed main(){
	vector<string> cases={"","a","ababa","abcdefg",string(80,'a'),string(3,'\0')};
	string bytes;
	for(int i=0;i<96;i++) bytes.push_back((char)((i*73+19)%256));
	cases.push_back(bytes);cases.push_back(string("\0\xff\x80\x01",4));
	SingleHash ha;Hash64 hb;bool wrapped=false;
	const __uint128_t mod64=(__uint128_t)1<<64;
	for(const auto &s:cases){
		ha.init("discarded");hb.init("discarded");ha.init(s);hb.init(s);
		require(ha.query(1,0)==0&&hb.query(1,0)==0,"empty interval after reinitialization");
		for(int l=1;l<=(int)s.size();l++){
			for(int r=l;r<=(int)s.size();r++){
				__uint128_t x=0,y=0;
				for(int i=l;i<=r;i++){
					unsigned c=(unsigned char)s[i-1]+1;
					x=(x*131+c)%1000000007;
					y=y*131+c;wrapped|=y>=mod64;y%=mod64;
				}
				require(ha.query(l,r)==(long long)x,"single modulus versus direct polynomial");
				require(hb.query(l,r)==(uint64_t)y,"natural wrap versus explicit 128-bit modulus");
			}
		}
	}
	require(wrapped,"cases exercise arithmetic beyond 64 bits");
	ha.init("ababa");hb.init("ababa");
	require(ha.same(1,3,3,5)&&hb.same(1,3,3,5),"equal substrings at different offsets");
	require(!ha.same(1,2,2,3)&&!hb.same(1,2,2,3),"different same-length substrings");
	require(!ha.same(1,2,1,3)&&!hb.same(1,2,1,3),"length check");
	require(ha.same(1,0,2,1)&&hb.same(1,0,2,1),"two empty intervals");
	SingleHash other;Hash64 other64;other.init("aba"),other64.init("aba");
	require(ha.query(3,5)==other.query(1,3)&&hb.query(3,5)==other64.query(1,3),"cross-string substring query");
	cout<<"PASS: single and natural-overflow hash checks ("<<checks<<")\n";
}
