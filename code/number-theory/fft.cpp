namespace FFT {
	const double PI = acos(-1);
	const int MAX_SIZE = 1 << 20; // 1M 支 持 up to 2^20

	struct Complex {
		double a, b;
		Complex(double x = 0, double y = 0) : a(x), b(y) {}
		Complex operator + (const Complex &rhs) const { return Complex(a + rhs.a, b + rhs.b); }
		Complex operator - (const Complex &rhs) const { return Complex(a - rhs.a, b - rhs.b); }
		Complex operator * (const Complex &rhs) const {
			return Complex(a * rhs.a - b * rhs.b, a * rhs.b + b * rhs.a);
		}
	};

	int rev[MAX_SIZE];

	inline Complex EXP(double theta) {
		return Complex(cos(theta), sin(theta));
	}

	void GetReverse(int len, int log_len) {
		for (int i = 0; i < len; ++i)
		rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (log_len - 1));
	}

	void FFT(vector<Complex > &a, int dir) {
		int len = a.size();
		for (int i = 0; i < len; ++i)
		if (i < rev[i]) swap(a[i], a[rev[i]]);

		for (int stp = 1; stp < len; stp <<= 1) {
			for (int k = 0; k < stp; ++k) {
				Complex wk = EXP(dir * k * PI / stp);
				for (int even = k; even < len; even += stp << 1) {
					int odd = even + stp;
					Complex tmp = a[odd] * wk;
					a[odd] = a[even] - tmp;
					a[even] = a[even] + tmp;
				}
			}
		}

		if (dir == -1) {
			for (int i = 0; i < len; ++i)
			a[i].a /= len;
		}
	}

	// 计 算 两 个 整 数 多 项 式 的 卷 积 结 果
	vector<int> multiply(const vector<int> &A, const vector<int> &B) {
		int n = A.size(), m = B.size();
		int len = 1, log_len = 0;
		while (len < n + m - 1) len <<= 1, log_len++;
		GetReverse(len, log_len);
		vector<Complex > a(len), b(len);
		for (int i = 0; i < n; ++i) a[i].a = A[i];
		for (int i = 0; i < m; ++i) b[i].a = B[i];

		FFT(a, 1);
		FFT(b, 1);
		for (int i = 0; i < len; ++i) a[i] = a[i] * b[i];
		FFT(a, -1);

		vector<int> res(n + m - 1);
		for (int i = 0; i < n + m - 1; ++i)
		res[i] = round(a[i].a);
		return res;
	}
}
