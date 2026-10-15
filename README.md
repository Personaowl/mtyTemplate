# mtyTemplate

按自己的码风维护的 ACM / ICPC 算法模板手册，采用墨蓝与青绿色的封面、目录和代码展示样式。只维护一个版本，日常阅读和打印使用同一份 PDF。

**PDF 只收录封面、目录和算法内容；模板使用及维护说明统一放在本 README。**

## 快速构建

在仓库根目录运行：

```powershell
.\scripts\build.ps1
```

也可以直接执行：

```sh
latexmk main.tex
```

唯一 PDF 产物为 `build/main.pdf`。PowerShell 脚本自动切换到项目根目录；若系统策略不允许执行脚本，直接使用 `latexmk` 即可。直接执行 `latexmk` 时应从仓库根目录运行。

## 环境与编辑器

- TeX Live，包含 XeLaTeX、latexmk、ctex、listings、tcolorbox、enumitem、fancyhdr 和常见数学包。
- 字体使用 TeX Live 自带的 Fandol 与 TeX Gyre，无需 Windows 专属字体。
- 使用 `listings` 展示代码，不依赖 Pygments，也不需要 shell escape。
- 代码采用 C++17；示范代码校验另需 Python 3 和 PATH 中的 `g++`，PDF 构建本身不需要 Python。
- Overleaf：选择 XeLaTeX，主文档选择 `main.tex`。
- VS Code：可安装推荐的 LaTeX Workshop 扩展，选择“编译手册”配方。

Ubuntu 精简安装需要显式安装 OpenType 字体包 `fonts-texgyre`；只安装 `texlive-fonts-recommended` 不应作为字体已齐全的保证。GitHub Actions 的安装列表包含该字体包与 `fontconfig`，安装后执行 `fc-cache -f`。若日志提示 `TeX Gyre Pagella`、`TeX Gyre Heros` 或 `TeX Gyre Cursor` 找不到，先检查这些依赖与字体缓存。

## 模板使用说明

### 怎样查阅和复用

新增算法条目依次给出用途、复杂度、接口约定、简练的原理讲解、使用教程与模板代码。使用教程说明初始化、调用顺序和返回值含义，给一组简单数据演示调用，不列预期输出；仅在确有必要时补充易错点。先核对下标、数据范围、整数类型和初始化要求，再复制实现。数论中的纯定理、计数模型与博弈结论保留适用条件、关键推导和公式，不强行配置代码或调用教程。来源与校验记录见条目注释及 `docs/validation.md`。

`code/` 保存唯一 C++ 源码，LaTeX 通过 `\CodeFile` 直接引用。修改算法时改对应 `.cpp`，无需在文档中同步修改第二份代码。

代码片段通常接在解题框架的全局声明之后使用；完整程序与特殊依赖会在条目中说明。多组数据题目需要按说明清理全局状态。

### 码风约定

- 用 `solve()` 组织题目逻辑，数组和节点通常从 1 开始编号。
- 使用简短竞赛命名，如 `fa,sz,dep,dfn,vis,tot,ans,res`。
- C++ 使用 Tab 缩进，左大括号同行；常见排版为 `for(int i=1;i<=n;i++){`。
- 数据结构优先使用 `struct` 封装；局部辅助函数可使用 lambda，递归常使用 `auto &&self`。
- 保留 `#define int long long` 配合 `signed main()` 的习惯；内存敏感模板单独说明类型。
- 64 位正无穷使用 `static constexpr long long INF=4e18`，负无穷使用 `-INF` 或 `-4e18`；真实值需满足条目约定，不能拿不可达哨兵直接参与转移。

更完整的约定见 [码风约定](docs/code-style.md)。

### 当前内容与后续分类

| 分类 | 当前内容 / 后续方向 |
| --- | --- |
| 基础 | 暂不收录 PDF，保留解题框架等源文件 |
| 数论（第二章） | 模运算、扩欧、逆元、筛法、欧拉降幂、Legendre、整除分块、组合计数、Lucas、康托、容斥、矩阵、高斯消元、CRT、线性基、博弈与 SG 状态图模板 |
| 数据结构 | 暂不收录 PDF，保留普通并查集等源文件 |
| 图论（第三章） | 最短路（含结构体堆写法）、最小生成树、分层图、同余最短路、拓扑排序与 DAG DP、SCC 与缩点、割点割边、边双、2-SAT、差分约束、最小环、最大流、费用流、最小割建模与上下界流 |
| 字符串（第一章） | Trie、双哈希、KMP、Z 函数与扩展 KMP、Manacher、AC 自动机 |
| DP / 几何 | 预留目录，收录真实条目后再启用 |

## 目录与文件职责

```text
mtyTemplate/
├── README.md                 # 模板使用与维护说明
├── main.tex                  # 唯一文档入口
├── config/
│   ├── metadata.tex          # 标题、作者、版本与版本日期
│   ├── style.tex             # 字体、颜色、页边距、页眉与代码样式
│   └── commands.tex          # 条目信息卡、源码引用与易错点环境
├── frontmatter/cover.tex     # 封面
├── chapters/
│   ├── contents.tex          # 分类顺序清单
│   ├── basics/               # 基础（暂不收录 PDF）
│   ├── number-theory/        # 数论及相关数学主题（第二章）
│   ├── data-structures/      # 数据结构（暂不收录 PDF）
│   ├── graphs/               # 图论（第三章）
│   ├── strings/              # 字符串算法
│   ├── dp/                   # 动态规划（预留）
│   └── geometry/             # 计算几何（预留）
├── code/                     # 唯一 C++ 源码，与说明分类对应
├── templates/                # 新增算法条目的样板
├── docs/                     # 详细码风与校验记录
├── scripts/                  # 构建与代码校验
├── tests/                    # 朴素对拍与行为校验
├── .github/workflows/        # GitHub 自动编译
└── build/                    # PDF 与编译文件，不纳入 Git
```

## 维护说明

### 添加一个算法

1. 将 `templates/algorithm.tex` 复制到 `chapters/<分类>/<名称>.tex`，填写用途、时间与空间复杂度、参数前提、下标、简练原理和使用教程。
2. 参考 `templates/algorithm.cpp`，把实现放入 `code/<分类>/<名称>.cpp`，保持原有命名和排版习惯。
3. 在说明中使用 `\CodeFile{code/<分类>/<名称>.cpp}` 引用源码，用一组简单数据演示调用方式，不列预期输出，必要时补充关键边界；来源写入条目注释，校验状态写入 `docs/validation.md`。
4. 在该分类的 `index.tex` 中加入 `\input{chapters/<分类>/<名称>}`。
5. 首次启用分类时，在 `chapters/contents.tex` 中加入对应分类；没有真实条目时不启用空分类。
6. 编译手册并检查日志，按需在本地确认目录、代码换行和跨页；代码或调用示例变动时先做编译与逻辑核对，有疑点再补充相关检查。

有代码的算法由 `.tex` 说明和唯一 `.cpp` 源码组成，纯原理条目仅维护 `.tex`。新增代码后同步相应编译检查的源码依赖顺序和条目清单，保证调用示例也参与编译。分类 `index.tex` 只负责分类标题与条目顺序，`chapters/contents.tex` 只负责分类顺序。

### 统一条目接口

```tex
\section{算法名称}
\label{algo:unique-name}
\AlgorithmInfo{用途}{时间与空间复杂度}{参数前提与下标}

\subsection*{原理}
简要说明核心思路，必要时给出公式。

\subsection*{使用教程}
说明初始化、调用顺序和返回值含义，给一组简单数据演示调用，不列预期输出。

\needspace{7\baselineskip}
\subsection*{模板代码}
\CodeFile{code/category/name.cpp}

% 有特殊注意事项时再补充一小段。
% 来源保存在条目注释，校验记录写入 docs/validation.md。
```

`\InlineCode{...}` 用于短路径、标识符和简单代码，不需要手工转义下划线；参数仍须遵守 TeX 花括号配对规则，不能直接包含 `%`。较长代码使用 `.cpp` 文件引用，数学公式正常使用 `$...$` 或 `\[...\]`。

### 代码和排版

- 每个算法只保留一份源码；普通并查集、可撤销并查集、带权并查集等不同性质的实现分开命名。
- 说明与边界跟随实现更新，不把题目特有逻辑直接搬进公共模板。模板代码不放 `assert`，输入范围与调用前提写入说明。
- 全局外观统一在 `config/style.tex` 调整，避免在单个算法条目中重复设置字体、颜色和页边距。
- 文档与源码使用 UTF-8，保持 TeX Live 自带字体；中文注释添加后按需在本地检查字距与断行。长链接使用 `\url{...}`。
- 长代码允许跨页，不要包在不可分页的浮动体中。源码行建议控制在约 80 个字符；长表达式在运算符附近分行，极长字符串使用 C++ 相邻字符串字面量分行。
- `listings` 按词法边界折行，极长单个标识符或无断点字符串仍可能溢出。出现 Overfull 等警告时要检查对应位置，不只看构建退出码。
- 使用与维护说明在本 README 更新，不再放入 PDF 或另维护一份相同内容的 LaTeX 章节。

### 版本、来源与校验

- 在 `config/metadata.tex` 更新版本号与版本日期，日期表示内容版本，不使用自动构建时间替代。
- 参考实现保留来源。未验证写“未验证”，只编译写“已编译”，随机对拍写明范围，在线评测 AC 写明题号或链接。
- 校验状态只描述已有证据；未经提交验证的实现不标注题目 AC。

默认采用“编译 + 代码核对”的流程，不在每次整理后重复对拍。每份模板仍必须核对实现与原理的一致性、关键不变量和边界；发现疑点先解决，再收录：

1. 新整理的实现优先参考已有通过验证的代码或可靠来源，保留出处。逐项核对状态含义、转移、下标、边界、整数范围、初始化及多测清理；核对实现是否满足算法不变量，不能以结构相似或编译通过代替逻辑审查。
2. 代码或调用示例变动后，运行 `python scripts/check_code.py`。当前会直接读取已收录章节的模板源码及 LaTeX 调用示例，并补齐其声明的基础依赖，分别在普通 `int` 和 `#define int long long` 下编译、链接，不执行对拍。
3. 纯文字、排版调整只编译 PDF。删除断言等不改变合法输入逻辑的修改做编译与核对即可，沿用此前的行为验证记录。
4. 新实现、关键逻辑变化或发现疑点时，先补查相关边界或相关算法；确需完整回归时，手动运行 `python scripts/check_code.py --full`。只运行字符串完整回归可用 `python scripts/check_strings.py --full`。保留测试方便定位问题，日常构建不自动执行。

`python scripts/check_strings.py` 只编译字符串；`python scripts/check_number_theory.py` 只编译数论；`python scripts/check_graphs.py` 只编译图论（含并查集依赖）。这些分类检查默认都不运行行为检查。新整理数论的少量固定边界与修正回归可手动运行 `python scripts/check_number_theory.py --smoke`，不枚举随机数据；完整回归入口 `--full` 也会包含这些固定检查。图论可手动运行 `python scripts/check_graphs.py --smoke` 检查固定边界，或用 `--audit` 在固定用例之外核对极小规模的独立模型；这些都不在日常编译中自动执行。既有验证范围与实际执行记录见 [校验记录](docs/validation.md)。

### 构建与清理

```sh
latexmk main.tex
latexmk -c main.tex
```

`-c` 清除辅助文件并保留 PDF。所有产物统一放在 `build/` 并被 Git 忽略，不移动到源码目录。`.latexmkrc` 统一选择 XeLaTeX、禁用 shell escape，并设置输出路径。

## Git 与发布

仓库分支为 `main`，远程 `origin` 已配置为 [Personaowl/mtyTemplate](https://github.com/Personaowl/mtyTemplate)。修改并检查后，按需提交和推送：

```sh
git add .
git commit -m "Describe the handbook update"
git push origin main
```

首次提交如提示身份信息，请自行配置 `user.name` 与 `user.email`。仓库尚未指定开源许可证，发布前请自行决定并核对引用代码的授权。

推送到 `main` 或创建 PR 后，GitHub Actions 会编译手册和已收录的字符串、数论、图论模板及调用示例，不自动运行完整对拍。构建成功后，PDF 在对应运行的 **Artifacts → mtyTemplate-pdf** 中下载，保留 30 天；远程检查状态以对应提交的 Actions 运行记录为准。

工作流不自动提交 PDF 或创建 Release。正式发布时可手工将 PDF 上传至 GitHub Release，并添加对应版本标签。
