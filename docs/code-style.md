# 码风约定

依据 `D:/codeforces/ACM-Training/daily-problems` 中 210 份 C++ 文件的整体扫描和 26 份代表代码的阅读归纳。后续以实际使用习惯为准，不把少数排版差异强行统一成新的风格。

- `#include<bits/stdc++.h>`、`using namespace std;`、`#define endl '\n'`。
- 题目逻辑放入 `void solve(){}`；主函数常用 `int T=1;while(T--) solve();`。
- 经常使用 `#define int long long` 配合 `signed main()`；内存敏感算法可以使用普通 `int` 并单独标注。
- C++ 使用 Tab 缩进（展示宽度 4），左大括号同行；常见排版是 `for(int i=1;i<=n;i++){`。
- 简单分支可单行，例如 `if(x==y) return false;`。大段逻辑分行书写。
- 默认 1 下标，常用 `vector<int> a(n+1)`；图用邻接表。按算法需要保留全局定长数组。
- 常用命名：`fa,sz,dep,dfn,vis,tot,cnt,ans,res,maxn,minn`。
- 常用函数：`init,dfs,lca,find,merge,modify,query,qpow`。
- 数据结构常封装成 `struct DSU/BIT/SegTree`；线段树用 `Node,tr,ls,rs,pushup,pushdown`。
- 局部辅助函数使用 `[&]` lambda，递归常使用 `auto &&self`；遍历可使用 `auto` 和结构化绑定。
- 中文说明主要放入 `.tex`，包括参数、返回值、范围、初始化、多测和特殊边界。源码中的必要短注释可以保留。

代码排版使用 `listings`，每次直接读取 `.cpp`；不在 LaTeX 内维护第二份实现。UTF-8 中文注释建议写成独立注释行，添加后检查实际 PDF 的字距与断行。
