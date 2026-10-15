namespace NTT {
	const int Mod = 998244353;
	const int G = 3;
	const int MaxSize = 1 << 20;
	int rev[MaxSize];
	inline int QuickPow(int a, int k) {
		int ret = 1;
		while (k) {
			if (k & 1) ret = 1LL * ret * a % Mod;
			a = 1LL * a * a % Mod;
			k >>= 1;
		}
		return ret;
	}
	inline void GetReverse(int len, int log_len) {
		for (int i = 0; i < len; ++i)
		rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (log_len - 1));
	}

	void NTT(vector<int> &a, int dir) {
		int len = a.size();
		for (int i = 0; i < len; ++i)
		if (i < rev[i]) swap(a[i], a[rev[i]]);

		int g = (dir == 1 ? G : QuickPow(G, Mod - 2));
		for (int stp = 1; stp < len; stp <<= 1) {
			int wn = QuickPow(g, (Mod - 1) / (stp << 1));
			for (int k = 0; k < stp; ++k) {
				int w = QuickPow(wn, k);
				for (int i = k; i < len; i += (stp << 1)) {
					int u = a[i], v = 1LL * a[i + stp] * w % Mod;
					a[i] = (u + v) % Mod;
					a[i + stp] = (u - v + Mod) % Mod;
				}
			}
		}

		if (dir == -1) {
			int inv_len = QuickPow(len, Mod - 2);
			for (int i = 0; i < len; ++i)
			a[i] = 1LL * a[i] * inv_len % Mod;
		}
	}

	vector<int> multiply(const vector<int>& A, const vector<int>& B) {
		int n = A.size(), m = B.size();
		int len = 1, log_len = 0;
		while (len < n + m - 1) len <<= 1, ++log_len;

		GetReverse(len, log_len);
		vector<int> a(len), b(len);
		copy(A.begin(), A.end(), a.begin());
		copy(B.begin(), B.end(), b.begin());
		NTT(a, 1);
		NTT(b, 1);
		for (int i = 0; i < len; ++i)
		a[i] = 1LL * a[i] * b[i] % Mod;
		NTT(a, -1);
		a.resize(n + m - 1);
		return a;
	}
}
