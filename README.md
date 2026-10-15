# mtyTemplate

猫头鹰的 ACM / ICPC 算法模板手册。源码与讲解分开维护，使用 C++17、XeLaTeX；阅读和打印共用一份 PDF。

当前版本 **v0.12.3**，共 **九章、175 个条目**。PDF 只含封面、目录与算法，使用和维护说明集中在本 README。

## 构建与下载

在仓库根目录运行：

```sh
latexmk main.tex
```

Windows 也可运行 `.\scripts\build.ps1`。唯一产物为 `build/main.pdf`；清理辅助文件并保留 PDF 用 `latexmk -c main.tex`。

- TeX Live 须包含 XeLaTeX、latexmk、ctex、listings、tcolorbox 及常见数学包。
- 字体采用 Fandol、TeX Gyre，不依赖 Windows 字体或 shell escape。Ubuntu 按 `.github/workflows/build.yml` 安装，须包含 `fonts-texgyre`、`fontconfig`，安装后执行 `fc-cache -f`。
- 代码检查另需 Python 3 和 PATH 中的 g++；PDF 构建本身不需要 Python。
- Overleaf 选择 XeLaTeX，主文件为 `main.tex`；VS Code 可用仓库附带的 LaTeX Workshop 配方。

推送后，可在 [GitHub Actions](https://github.com/Personaowl/mtyTemplate/actions) 对应运行的 **Artifacts → mtyTemplate-pdf** 下载 PDF，保留 30 天。工作流只编译，不自动对拍、提交 PDF 或发布 Release。

## 内容目录

| 章 | 内容 | 条目数 |
| --- | --- | ---: |
| 1 字符串 | Trie、AC 自动机、三种哈希、KMP、Z／扩展 KMP、Manacher | 8 |
| 2 数论 | 同余、数论函数与反演、组合计数、插值／FFT／NTT、矩阵／线性代数、博弈 | 22 |
| 3 图论 | 最短路、生成树、DAG DP、Tarjan、欧拉路径、2-SAT、二分图、网络流 | 25 |
| 4 树 | 性质、直径／中心／重心、LCA／差分、重链／长链、虚树、重构树、DSU on tree | 16 |
| 5 STL | 容器、迭代器、比较器／运算符重载、排序查找、位集、GNU 扩展 | 18 |
| 6 数据结构 | 并查集、单调结构／对顶堆、01 Trie、ST／BIT、线段树、可持久化、分块／ODT／莫队 | 26 |
| 7 动态规划 | 背包、区间、状压、数位、树形、概率／期望、单调结构／凸包／分治优化 | 26 |
| 8 计算几何 | 精度、直线／线段、多边形／凸包、圆、半平面交、三维基础 | 23 |
| 9 其他 | 起手模板、二分／三分、折半搜索、多维前缀／差分、生成器与比赛脚本 | 11 |

各章按“章 → 主题 → 算法”组织。树章维护树的结构与查询，树剖加线段树归数据结构；数论同时含组合、线性代数、博弈。基础目录不单独启用，起手程序由第九章引用。

## 怎样使用模板

先读用途和适用条件，再看教程并复制源码。条目给出原理、接口含义和一组调用数据，不列预期输出；数论保留必要推导，DP 明确状态、初值、转移顺序和答案位置，几何逐项说明公开函数与退化返回值。

- `.cpp` 通常接在头文件和命名空间声明之后，先复制依赖，再复制目标实现。起手模板、随机生成器是独立程序。
- 有共享状态才用结构体，通过 `obj.init(...)`、`obj.query(...)` 调用；独立计算与普通 DP 保留函数。
- 按教程初始化、加数据、构建，再查询。`build/run/solve` 重用规则各不相同，不要假定它们都会清空原状态。
- 通常为 1 下标、闭区间；STL、几何、状压等例外逐项标明。STL 的 `[first,last)` 不含末项。
- 支持普通 int 和整数宏；使用 `#define int long long` 时主函数用 `signed main()`，大节点池另估内存。大对象用引用传递。
- 64 位哨兵通常为 `4e18`，真实值须满足范围，不可达状态先判定再转移。128 位须在乘前提升，输出可用 STL 的 `str128`。

### 容易混淆的接口

| 模板 | 使用时确认 |
| --- | --- |
| 并查集 | 普通 merge 返回“是否新合并”；带权、种类 merge 返回“是否相容”。方向为 `a[y]-a[x]=w`、`c[y]-c[x]=r (mod k)`，未知先读状态 |
| 静态线段树 | 填 w 后 `build(1,1,n)`，query 返回 Node。修改不更新 w，再次 build 会重建；公共约定在线段树主题开头 |
| BIT／01 Trie | BIT 的 kth 要求当前各位置频次非负；01 Trie 返回异或值，删除不回收历史节点 |
| LCA／HLD／虚树 | 无权 LCA 用 add(u,v)；HLD 边模式排除 LCA，路径拼接要保留方向。虚树先 prepare，再逐轮 build(keys) |
| 网络流 | flow 修改残量，返回本次增广量；费用流返回状态、流量、费用。跑满后才可恢复最小割 |
| 欧拉／点双 | 欧拉无向边只加一次，递归深度按边数计；点双用 belong[u] 保存多重归属，自环忽略 |
| 莫比乌斯 | 复用 Sieve 的 mu/pre_mu；分块不能省掉实际求和上界的筛表，计数用 128 位 |
| DP 凸包 | 原生 query 求最大值；最小值用负系数插入、结果取负，单调版另核对转换后的顺序 |
| 计算几何 | 整数点、实数点分开；-1/0/1 的位置判定不能直接转 bool，交点／optional／无效圆先读状态，排序不加 EPS |

共享源码只复制一次：普通 DSU 供图论、重构树等使用，TreeLCA 供树差分、长链、虚树使用，HLD 供树剖维护和 DSU on tree 使用。BIT 应用的 `solve_p1972()`、`solve_p2163()` 和树剖维护的 `solve_hld()` 已带题目读入输出，可从自己的 main 调用。

FFT、NTT、素数测试保留指定的原实现：FFT／NTT 的空输入、两个常数在调用处处理，直接变换长度须为 2..2^20；FFT 另核对输出范围与精度，NTT 固定模数 998244353。素数源码前需 `using i64=long long;`，确定性判素范围为 `n<3825123056546413051`；分解只传正数并受同一范围约束。完整限制见各条目。

## 比赛脚本

将 `contest/` 中需要的脚本复制到题目目录。对拍同时准备 gen.cpp（生成器）、brute.cpp（参考解）、main.cpp（待测程序）。

```sh
# Ubuntu：编译指定文件，从 in.txt 读取，向 out.txt 输出
bash run.sh a.cpp

# Linux 对拍：最多 1000 轮，每程序每轮限时 3 秒
bash stress.sh 1000 3
```

Windows 在 CMD 用 `stress.bat 1000`，PowerShell 用 `.\stress.bat 1000`。批处理不设单轮超时，死循环用 Ctrl+C 中断。两版都会先编译、检查三个程序的运行状态，首次不一致即停止；Linux 另需 Bash、timeout、diff。

每轮编号作为种子，保留 `stress.seed`、`stress.in`、`stress.ans`、`stress.out`、`stress.diff` 及三个 stderr 文件，重跑前先另存。Linux 用 diff 严格比较文本，Windows 用 `fc.exe /b` 比较字节；浮点容差、多解、SPJ 另配判定器。跨标准库重放未必相同，以保留输入为准。

run.sh 只维护两行，唯一参数为带 .cpp 的文件名，路径含空格时加引号。编译成功才运行并覆盖 out.txt，不增加参数检查、计时或其他选项。

## 维护方式

### 文件职责

| 路径 | 职责 |
| --- | --- |
| main.tex／chapters/contents.tex | 唯一入口／章节顺序 |
| chapters/分类/index.tex | 主题与条目顺序 |
| chapters/分类/算法.tex | 原理、约定与调用示例 |
| code/分类/算法.cpp | 唯一算法源码 |
| config/ | 元数据、样式、公共命令 |
| frontmatter/cover.tex、assets/ | 封面布局与原图 |
| templates/ | 新条目样板 |
| scripts/、tests/、contest/ | 编译检查、专项验证、比赛脚本 |
| docs/code-style.md、docs/validation.md | 码风细则、来源与实际校验记录 |
| build/ | 生成文件，Git 忽略 |

### 新增或修改条目

1. 参考 `templates/algorithm.tex` 写讲解，说明用途、复杂度、范围、下标与调用顺序。
2. 可复用实现只存一份 `.cpp`，用 `\CodeFile{code/分类/算法.cpp}` 引用；依赖已有实现时不复制第二套。
3. 在分类 index.tex 加 input；启用真实新章节时再改 contents.tex。主题用 section、算法用 subsection、内部讲解用带星 subsubsection。
4. 同步该章编译脚本的源码依赖顺序与条目清单，保证调用示例参与检查。来源写注释，实际验证写 validation.md。
5. 编译并检查日志；代码或调用变动时做代码编译，新增逻辑或疑点只检查相关算法。

纯定理、模型和 STL 直接调用不建立空源码。教程只讲解题会直接使用的接口，内部 DFS、消元、推流不逐项展开；几何按既定要求保留详细接口及逐函数用途注释。

代码使用 Tab、同行左大括号、简短竞赛命名；图／树 DFS 用递归，算法所需点栈、单调栈正常保留。不批量格式化既有实现，不放 assert，合法输入前提写说明；FFT／NTT／素数测试的原算法语句不擅自改动。

文档用 UTF-8，全局外观在 `config/style.tex` 调整，长代码允许跨页。`\InlineCode{...}` 可写下划线，仍须配对花括号，不能直接包含 `%`。检查 Overfull／Underfull、缺字、字体、未定义引用警告；当前按约定不自动打开预览。

### 封面与版本

`config/metadata.tex` 维护标题、署名、Codeforces／GitHub、图片路径、版本和日期。两行标题为 BookTitlePrefix、BookTitleMain，组合的 BookTitle 用于 PDF 属性；日期表示内容版本，不自动跟随构建时间。

主标题 44 pt、副标题和链接 14 pt、署名 18 pt。主体左边距 26 mm，以 `frontmatter/cover.tex` 的 `CoverBodyOffset=25mm` 统一下移；标签与头像距顶 51 mm，两行标题距顶 73／96 mm。头像、ICPC 图标均宽 52 mm，分别使用 assets/avatar.jpg、assets/icpc-foundation.jpg 原图。链接为蓝色无衬线字体、固定标签列对齐；竖条和右下版本独立定位。只改封面时重新编译 PDF 即可。

## 校验命令

```sh
# 默认只编译源码、依赖与全部调用示例
python scripts/check_code.py

# 显式运行已有完整行为回归
python scripts/check_code.py --full
```

默认按普通 int 与整数宏两种模式编译、链接，不执行生成程序；起手模板、生成器单独按普通 main 编译。仍需核对实现与原理、边界、范围、初始化和不变量，不能用结构相似或编译通过代替逻辑核对。纯文字／排版调整只编译 PDF，不重跑无关对拍。

| 范围 | 命令／选项 |
| --- | --- |
| 单章编译 | `python scripts/check_<分类>.py`；分类名见 scripts/，如 graphs、number_theory、data_structures |
| 单章固定／专项回归 | 数论、图论、树、STL、数据结构、DP、几何、其他可加 `--smoke`；字符串加 `--full` |
| 图论／树的极小模型 | check_graphs.py、check_trees.py 加 `--audit`，完整入口不默认附加这两项 |
| 01 Trie／0-1 BFS／欧拉／点双／反演 | `python scripts/check_basic_additions.py`，也含在完整回归中 |
| 哈希 | `python scripts/check_hash.py`，也含在字符串完整回归中 |
| 闭合子图／面积／插值卷积判素 | 分别用 check_graphs.py 的 `--closure`、check_geometry.py 的 `--area`、check_number_theory.py 的 `--additions` |
| 比赛脚本 | `python scripts/check_contest.py`；只查简版 run.sh 加 `--run-only`，Windows 需 Git Bash |

专项按实际覆盖范围记录，不把本地检查标注为在线评测 AC；历史结果保留在 [校验记录](docs/validation.md)。

几何专项包含半平面交的近乎平行边界，以及圆与多边形交面积的相切、近切和旋转用例；解析面积与独立参考模型共同核对，避免只检查一般位置。

## 提交与远程构建

远程为 [Personaowl/mtyTemplate](https://github.com/Personaowl/mtyTemplate)，分支 main。完成修改与检查后：

```sh
git add .
git commit -m "Describe the handbook update"
git push origin main
```

build/ 不提交。远程构建以对应提交的 Actions 结果为准；若找不到 TeX Gyre 字体，检查 Ubuntu 字体包与缓存。整体仓库尚未指定开源许可证，参考出处保留在条目注释中。
