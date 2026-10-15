// Minimum insertions needed to make the entire string a palindrome.
int palindrome_insertions(const string& str){
	int n=str.size();string s=" "+str;
	vector<vector<int>> f(n+2,vector<int>(n+2));
	for(int len=2;len<=n;len++) for(int l=1;l+len-1<=n;l++){
		int r=l+len-1;
		if(s[l]==s[r]) f[l][r]=f[l+1][r-1];
		else f[l][r]=min(f[l+1][r],f[l][r-1])+1;
	}
	return f[1][n];
}
