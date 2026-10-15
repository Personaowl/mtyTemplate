#include<bits/stdc++.h>
#ifdef MTY_INT64
#define int long long
#endif
using namespace std;

#include "code/strings/trie.cpp"
#include "code/strings/hash.cpp"
#include "code/strings/kmp.cpp"
#include "code/strings/exkmp.cpp"
#include "code/strings/manacher.cpp"
#include "code/strings/ac_automaton.cpp"

long long checks=0;
mt19937 rng(20261015);
void require(bool ok,const char *expr,int line){
	checks++;
	if(!ok){
		cerr<<"FAIL at line "<<line<<": "<<expr<<'\n';
		exit(1);
	}
}
#define REQUIRE(x) require((x),#x,__LINE__)

string random_string(int maxn,const string &alphabet){
	string s;
	int n=rng()%(maxn+1);
	while(n--) s+=alphabet[rng()%alphabet.size()];
	return s;
}
vector<string> enumerate(int maxn){
	vector<string> res(1,"");
	int l=0,r=1;
	for(int len=1;len<=maxn;len++){
		for(int i=l;i<r;i++){
			res.push_back(res[i]+'a');
			res.push_back(res[i]+'b');
		}
		l=r,r=res.size();
	}
	return res;
}
vector<int> naive_match(const string &s,const string &t){
	vector<int> ans;
	for(int i=0;i+(int)t.size()<=(int)s.size();i++){
		if(s.compare(i,t.size(),t)==0) ans.push_back(i+1);
	}
	return ans;
}
int naive_lcp(const string &s,const string &t,int i){
	int len=0;
	while(i+len<(int)s.size()&&len<(int)t.size()&&s[i+len]==t[len]) len++;
	return len;
}
bool palindrome(const string &s,int l,int r){
	while(l<r){
		if(s[l++]!=s[r--]) return false;
	}
	return true;
}

void check_single(const string &s){
	int n=s.size();
	auto pi=prefix_function(s);
	auto z=z_function(s);
	REQUIRE((int)pi.size()==n+1);
	REQUIRE((int)z.size()==n+1);
	for(int i=1;i<=n;i++){
		int best=0;
		for(int len=1;len<i;len++){
			if(s.substr(0,len)==s.substr(i-len,len)) best=len;
		}
		REQUIRE(pi[i]==best);
		REQUIRE(z[i]==naive_lcp(s,s,i-1));
	}
	StringHash ha;
	ha.init("discarded");
	ha.init(s);
	Manacher ma;
	ma.init("discarded");
	ma.init(s);
	int best=0,pos=1;
	long long total=0;
	for(int l=1;l<=n;l++){
		for(int r=l;r<=n;r++){
			long long h1=0,h2=0;
			for(int i=l;i<=r;i++){
				int c=(unsigned char)s[i-1]+1;
				h1=(h1*131+c)%1000000007;
				h2=(h2*499+c)%1000000009;
			}
			auto h=ha.query(l,r);
			REQUIRE(h.first==h1&&h.second==h2);
			bool ok=palindrome(s,l-1,r-1);
			REQUIRE(ma.check(l,r)==ok);
			if(ok){
				total++;
				if(r-l+1>best){best=r-l+1;pos=l;}
			}
		}
	}
	REQUIRE(ma.len==best&&ma.pos==pos);
	REQUIRE(ma.count()==total);
	REQUIRE(ma.check(1,0));
	REQUIRE(ha.query(1,0)==make_pair(0ll,0ll));
	for(int i=0;i<25&&n;i++){
		int l1=rng()%n,r1=rng()%n,l2=rng()%n,r2=rng()%n;
		if(l1>r1) swap(l1,r1);
		if(l2>r2) swap(l2,r2);
		REQUIRE(ha.same(l1+1,r1+1,l2+1,r2+1)==(s.substr(l1,r1-l1+1)==s.substr(l2,r2-l2+1)));
	}
}

void check_pair(const string &s,const string &t){
	REQUIRE(kmp(s,t)==naive_match(s,t));
	auto e=exkmp(s,t);
	REQUIRE(e.size()==s.size()+1);
	for(int i=1;i<=(int)s.size();i++) REQUIRE(e[i]==naive_lcp(s,t,i-1));
}

void check_trie(){
	Trie tr;
	map<string,int> freq;
	for(int step=0;step<3500;step++){
		if(step%401==0){tr.init();freq.clear();}
		string s=random_string(8,"abAB09");
		int op=rng()%4;
		if(op==0){tr.insert(s);freq[s]++;}
		if(op==1){
			bool ok=freq[s]>0;
			REQUIRE(tr.erase(s)==ok);
			if(ok) freq[s]--;
		}
		REQUIRE(tr.query(s)==freq[s]);
		int expected=0,total=0;
		for(auto [word,cnt]:freq){
			total+=cnt;
			if(word.compare(0,s.size(),s)==0) expected+=cnt;
		}
		REQUIRE(tr.prefix(s)==expected);
		REQUIRE(tr.prefix("")==total);
	}
	tr.init();
	string s(100000,'a');
	tr.insert(s);tr.insert(s);
	REQUIRE(tr.query(s)==2&&tr.prefix(s.substr(0,99999))==2);
	REQUIRE(tr.erase(s)&&tr.query(s)==1);
	tr.init();
	REQUIRE(tr.prefix("")==0&&tr.query(s)==0);
}

void check_ac(){
	AC ac;
	for(int round=0;round<200;round++){
		ac.init();
		vector<string> patterns(1,"");
		int k=rng()%16;
		for(int i=1;i<=k;i++){
			string s=random_string(7,"abc");
			patterns.push_back(s);
			REQUIRE(ac.insert(s)==i);
		}
		ac.build();ac.build();
		for(int q=0;q<8;q++){
			string s=random_string(24,"abc");
			auto ans=ac.query(s);
			REQUIRE(ans.size()==patterns.size());
			for(int i=1;i<=k;i++) REQUIRE(ans[i]==(long long)naive_match(s,patterns[i]).size());
		}
	}
	ac.init();
	for(int i=1;i<=1000;i++) ac.insert(string(i,'a'));
	ac.build();
	auto ans=ac.query(string(200000,'a'));
	for(int i=1;i<=1000;i++) REQUIRE(ans[i]==200000-i+1);
	REQUIRE(ac.query("a")[1]==1);
}

void check_examples(){
	Trie tr;tr.insert("abc");tr.insert("abd");
	REQUIRE(tr.query("abc")==1&&tr.prefix("ab")==2);
	REQUIRE(tr.erase("abc")&&tr.query("abc")==0);
	StringHash ha;ha.init("ababa");
	REQUIRE(ha.same(1,3,3,5)&&!ha.same(1,2,2,3));
	REQUIRE(kmp("ababa","aba")==vector<int>({1,3}));
	REQUIRE(exkmp("ababa","aba")==vector<int>({0,3,0,3,0,1}));
	Manacher ma;ma.init("cabbad");
	REQUIRE(ma.len==4&&ma.pos==2&&ma.check(2,5)&&ma.count()==8);
	AC ac;ac.insert("a");ac.insert("aa");ac.insert("a");ac.build();
	REQUIRE(ac.query("aaa")==vector<long long>({0,3,2,3}));
}

int32_t main(){
	check_examples();
	auto texts=enumerate(8),patterns=enumerate(5);
	for(auto &s:texts){
		check_single(s);
		for(auto &t:patterns) check_pair(s,t);
	}
	string alphabet="ab#";
	alphabet.push_back('\0');
	alphabet.push_back((char)128);
	alphabet.push_back((char)255);
	for(int i=0;i<600;i++){
		string s=random_string(24,alphabet),t=random_string(12,alphabet);
		check_single(s);check_pair(s,t);
	}
	check_trie();check_ac();
	string s(100000,'a');
	Manacher ma;ma.init(s);
	REQUIRE(ma.len==100000&&ma.count()==5000050000ll&&ma.check(1,100000));
	auto pos=kmp(s,string(1000,'a'));
	REQUIRE(pos.size()==99001&&pos.front()==1&&pos.back()==99001);
	auto z=z_function(s);
	for(int i=1;i<=100000;i++) REQUIRE(z[i]==100000-i+1);
	cout<<"PASS: strings, sizeof(int)="<<sizeof(int)<<", "<<checks<<" checks\n";
}
