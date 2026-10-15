# 码风约定

依据 `D:/codeforces/ACM-Training/daily-problems` 中 210 份 C++ 文件的整体扫描和 26 份代表代码的阅读归纳。后续以实际使用习惯为准，不把少数排版差异强行统一成新的风格。

- `#include<bits/stdc++.h>`、`using namespace std;`、`#define endl '\n'`。
- 题目逻辑放入 `void solve(){}`；主函数常用 `int T=1;while(T--) solve();`。
- 经常使用 `#define int long long` 配合 `signed main()`；内存敏感算法可以使用普通 `int` 并单独标注。
- C++ 使用 Tab 缩进（展示宽度 4），左大括号同行；常见排版是 `for(int i=1;i<=n;i++){`。
- 简单分支可单行，例如 `if(x==y) return false;`。大段逻辑分行书写。
- 计算几何循环点集／多边形使用 0 下标、末尾不重复首点；整数 IPoint 与实数 Point 分开，前者谓词用 __int128，后者 long double；排序器不使用 EPS，长度容差与面积、角度阈值分别处理。独立计算保留普通函数，点／线／圆按自然数据类型封装。
- 默认 1 下标，常用 `vector<int> a(n+1)`；图用邻接表。按算法需要保留全局定长数组。
- 常用命名：`fa,sz,dep,dfn,vis,tot,cnt,ans,res,maxn,minn`。
- 常用函数：`init,dfs,lca,find,merge,modify,query,qpow`。
- 数据结构常封装成 `struct DSU/BIT/SegTree`；线段树用 `Node,tr,w,ls,rs,eval,pushup,pushdown`，保留 `pushup(Node&,const Node&,const Node&)` 与 `pushup(int)` 两层，懒标记的 `pushdown` 同理；递归 `build(1,1,n)`，`query` 返回 `Node`，读 `.sum/.maxn` 等信息。不同功能使用独立结构体名，不套额外泛型框架；`ls/rs` 宏在实现末尾取消。
- DP 优先直接写状态数组、转移循环与递归函数，初始化在调用处明确给出；不为每个 DP 额外套结构体。凸包等独立数据结构仍按需要封装。讲解与代码中的状态、初值、转移顺序和答案位置应逐项对应。
- 局部辅助函数使用 `[&]` lambda，递归常使用 `auto &&self`；遍历可使用 `auto` 和结构化绑定。
- 图／树的 DFS、Tarjan 和 Dinic 推流用递归；不写显式调用栈来模拟 DFS。Tarjan 用于归属 SCC 的点栈仍保留；BFS、Kahn 拓扑排序使用队列。该约定适用于后续新增内容。
- 中文说明主要放入 `.tex`，包括参数、返回值、范围、初始化、多测和特殊边界。源码中的必要短注释可以保留。

代码排版使用 `listings`，每次直接读取 `.cpp`；不在 LaTeX 内维护第二份实现。UTF-8 中文注释建议写成独立注释行，添加后检查实际 PDF 的字距与断行。

STL 的直接调用及短比较类型示例保留在 `.tex`；可复用函数仍放唯一 `.cpp`。九章的全部调用示例使用 Tab 缩进，并由共享编译入口提取，不依赖某个教程标题或固定位置。

其他章起手程序按用户给出的普通 `int main()` 与读 T 形式原样维护；完整程序独立编译，只有可复用函数参与整数宏模式。数论 FFT、NTT 和素数测试按明确要求保留算法语句和命名，PDF 行号／空白恢复不作为重新改写；缺失类型别名在调用环境补齐，限制在使用教程中写明。比赛 shell 用 LF，Windows 批处理用 CRLF。
