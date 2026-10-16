# mtyTemplate

猫头鹰的 ACM / ICPC 算法模板手册。源码与讲解分开维护，使用 C++17、XeLaTeX；阅读和打印共用一份 PDF。

当前版本 **v0.12.5**，共 **九章、176 个条目**。PDF 只含封面、目录与算法，使用和维护说明集中在本 README。

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
| 6 数据结构 | 并查集、单调结构／对顶堆、01 Trie、ST／BIT、线段树／时间分治、可持久化、分块／ODT／莫队 | 27 |
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

## 维护与更新

以下操作均在仓库根目录 `D:\codeforces\mtyTemplate` 进行。章节、主题、算法编号由 LaTeX 自动生成；新增内容主要维护文件和引用顺序，不手工修改编号。

### 文件职责

| 路径 | 职责 |
| --- | --- |
| main.tex | 唯一文档入口，日常增补内容通常不用修改 |
| chapters/contents.tex | 启用哪些章节，以及整章的排列顺序 |
| chapters/分类/index.tex | 该章标题、主题分组，以及算法条目顺序 |
| chapters/分类/算法.tex | 算法标题、原理、前提、调用示例和源码引用 |
| code/分类/算法.cpp | 算法实现；PDF 直接读取此文件 |
| scripts/check_分类.py | 该章参与编译的源码、依赖顺序和说明文件清单 |
| scripts/check_code.py | 全书检查入口，新章的检查脚本在这里登记 |
| tests/ | 确有需要时使用的固定用例、独立模型与行为回归 |
| config/metadata.tex | 标题、版本、日期、作者、链接和封面图片路径 |
| config/style.tex、config/commands.tex | 字体、颜色、间距、代码样式、公共排版命令 |
| frontmatter/cover.tex、assets/ | 封面布局与原图 |
| templates/algorithm.tex | 新算法条目的讲解样板 |
| contest/ | 比赛使用的编译和对拍脚本 |
| docs/code-style.md、docs/validation.md | 码风细则、实际校验记录 |
| build/ | 生成文件，Git 忽略 |

### 一、修改已有条目

以 KMP 为例，讲解在 `chapters/strings/kmp.tex`，实现位于 `code/strings/kmp.cpp`。

1. **只补充原理、公式或使用说明**：编辑对应 `.tex`；条目顺序不变时不用改 index.tex。
2. **修改模板实现**：编辑对应 `.cpp`。它通过 `\CodeFile` 被 PDF 引用，重新构建即可更新书中代码，不需要再复制进 `.tex`。
3. **修改函数名、参数或返回值**：同步修改 `.tex` 的接口说明和调用示例；若其他模板依赖这个接口，还要更新调用方与相关检查。
4. **修改依赖或文件路径**：同步更新 `\CodeFile`、章节 index.tex 以及该章检查脚本。共享实现只维护一份，依赖在编译清单中排在使用者之前。

文字修改后运行 `latexmk main.tex`。代码或调用示例变动后，再运行 `python scripts/check_code.py`；算法逻辑发生变化或发现疑点时，按下文运行相关专项并记录结果。

### 二、在现有章节增加一个算法

以下以将来向字符串章增加 `minimal_rotation`（最小表示法）为例，文件名和函数名按实际实现调整。

**1. 建立源码与讲解文件。**

```text
code/strings/minimal_rotation.cpp
chapters/strings/minimal_rotation.tex
```

`.cpp` 存放可复用实现，通常不带头文件、`using namespace std` 和 main，与本章已有文件保持一致；依赖的类型或函数在文件开头注明。独立题解程序按独立程序处理，不能直接拼进含 main 的示例驱动。

讲解文件可在 PowerShell 中复制样板后编辑：

```powershell
Copy-Item templates/algorithm.tex chapters/strings/minimal_rotation.tex
```

替换样板中的 TODO，并填写：

- `\subsection{最小表示法}`：PDF 中的算法标题。
- `\label{str:minimal-rotation}`：全书唯一的引用标识，不写章节编号。
- `\AlgorithmInfo`：用途、复杂度、下标与参数范围。
- 原理和使用教程：核心思路、调用顺序、参数、返回值及必要的重置约定。
- 一组简单调用：放在 `lstlisting` 中，不列预期输出，不在其中复制整个算法实现。
- `\CodeFile{code/strings/minimal_rotation.cpp}`：创建源码后取消样板中这行的注释，并填好路径。

例如，若实现提供 `minimal_rotation(s)`，调用块可写为：

```latex
\begin{lstlisting}[style=mtycpp,numbers=none]
string s="baca";
int pos=minimal_rotation(s);
\end{lstlisting}
```

教程需说明 pos 的下标与含义。来源写在 `.tex` 注释中，实际执行过的检查写入 `docs/validation.md`。

**2. 把条目加入该章目录。**

编辑 `chapters/strings/index.tex`，在合适主题的现有 `\input` 旁加入：

```latex
\input{chapters/strings/minimal_rotation}
```

这一行的位置就是条目在 PDF 中的位置。如果需要新的主题分组，在该章 `AlgorithmGroups` 环境中新增 `\section{主题名称}`，再放入对应条目的 input。现有章节增加条目时，`chapters/contents.tex` 不用改。

**3. 把源码与示例加入编译检查。**

编辑 `scripts/check_strings.py`，在现有 `NAMES` 元组中追加 `'minimal_rotation'`。该脚本会据此同时读取同名 `.cpp` 与 `.tex`，编译源码和教程调用。

其他章节的清单形式可能不同：有的分别维护 `NAMES`、`CHAPTERS`、`SOURCES`。按该脚本实际结构，确保新增源码与讲解文件都在清单中；依赖先列、使用者后列，同一源码在一次编译中只加入一次。若需要额外类型别名，可参考 `compile_examples(..., preamble=...)` 的现有用法。

纯定理和没有可复用源码的条目只写 `.tex`；有调用示例则仍需纳入适用的示例检查。`compile_examples` 要求收录源码的说明文件带有调用示例，不能只加入一行 CodeFile。

**4. 检查并同步信息。**

```sh
python scripts/check_strings.py
latexmk main.tex
```

新增实现还需核对状态、边界和范围，有必要时补充独立用例。完成后更新 README 中本章条目数、总条目数，以及 `docs/validation.md` 的实际校验记录；内容版本需要发布时再更新 `config/metadata.tex`。提交前运行一次全书默认编译，确认各章和共用依赖均能编译。

### 三、建立一个全新章节

以下用 `new-topic` 表示新章目录，`example` 表示首个算法条目；将它们替换为实际名称。目录名可用短横线，Python 检查脚本与导入名使用下划线。

**1. 准备章节文件。**

```text
chapters/new-topic/
    index.tex
    example.tex
code/new-topic/
    example.cpp
scripts/check_new_topic.py
```

首个条目的 `.tex`、`.cpp` 按上一节建立。`chapters/new-topic/index.tex` 可写为：

```latex
\chapter{新章节标题}
\ChapterIntro{这一章收录的内容、共同约定与使用前提。}
\begin{AlgorithmGroups}
\section{第一个主题}
\input{chapters/new-topic/example}
\end{AlgorithmGroups}
```

层级统一为：章用 `\chapter`，主题用 `\section`，算法条目用 `\subsection`，条目内的原理、教程等用 `\subsubsection*`。不要在每个算法文件中再写一个 chapter，也不为尚无内容的分类启用空章节。

**2. 启用整章并决定位置。**

在 `chapters/contents.tex` 的目标位置加入：

```latex
\input{chapters/new-topic/index}
```

只有这个文件管理整章顺序。调整 input 的排列即可移动章节；main.tex 保持唯一入口，章节号与目录页码由重新构建更新。

**3. 建立该章的检查入口。**

有源码或调用示例的章节需要编译检查。下面是可参考的最小 `scripts/check_new_topic.py`：

```python
from pathlib import Path
import tempfile
from compile_examples import compile_examples

SOURCES = ['code/new-topic/example.cpp']
CHAPTERS = ['chapters/new-topic/example.tex']


def check(temp, smoke=False):
    compile_examples(temp, 'new_topic', SOURCES, CHAPTERS)
    # 后续增加行为验证时，在此根据 smoke 调用相关检查。


if __name__ == '__main__':
    with tempfile.TemporaryDirectory(prefix='mty-new-topic-') as directory:
        check(Path(directory))
```

这个最小入口只编译和链接，`smoke` 预留给全书完整回归调用。需要行为检查时，在 `tests/` 增加用例并接入该函数；需要命令行 `--smoke` 时，再参考已有检查脚本加入 argparse。纯理论且没有源码或调用块的章节可以跳过本步骤，包括下面的脚本登记；下一步只运行全书现有检查与 PDF 构建。

然后编辑 `scripts/check_code.py`，在已有章节导入旁加入：

```python
from check_new_topic import check as check_new_topic
```

在现有 `with tempfile.TemporaryDirectory(...)` 中，与其他章节检查并列加入：

```python
check_new_topic(Path(directory), smoke=args.full)
```

这样全书默认编译和 GitHub Actions 就会覆盖新章。只新增一个检查脚本而不登记，全书入口不会自动执行它。GitHub 工作流已经调用全书入口，通常无需为新章再增加单独步骤。

**4. 完成检查与目录信息更新。**

```sh
python scripts/check_new_topic.py
python scripts/check_code.py
latexmk main.tex
```

更新 README 的章节表、章节数、条目数，补充验证记录；若改变了分类约定，也同步 `AGENTS.md` 中的章节说明。版本与日期在 `config/metadata.tex` 维护。最后按“提交与远程构建”上传。

### 四、移动、改名与引用

移动条目主要改原章和目标章 index.tex 的 input。文件本身改名或换目录时，还要同步所有 input、CodeFile、依赖清单和示例检查路径；跨章依赖仍指向同一份实现。

正文引用使用 `第~\ref{str:minimal-rotation}~节` 一类 label/ref，避免写死“第几章第几节”。label 只要全书唯一就可以保留，移动章节不必跟着改名；确需改 label 时，搜索并更新所有引用。目录和交叉引用通过 latexmk 多轮构建自动刷新。

### 五、检查与记录如何选择

| 本次修改 | 应做的检查 |
| --- | --- |
| README、维护记录 | 核对链接、路径、命令与实际文件一致 |
| PDF 正文、公式、排版 | `latexmk main.tex`，检查最终日志 |
| 源码或教程中的 C++ 调用 | 全书默认 `python scripts/check_code.py`，再构建 PDF |
| 新实现、关键逻辑或发现疑点 | 在默认编译之外，核对不变量并运行相关专项，必要时补独立模型或反例 |
| 确需全面行为回归 | 显式运行 `python scripts/check_code.py --full` |

专项命令见下方“校验命令”。默认检查会在普通 int 和整数宏两种模式下编译、链接，不运行程序；新增行为用例必须接入相关脚本才会执行，不能仅把文件放进 tests/ 就认为已验证。

在 `docs/validation.md` 记录日期、涉及条目、实际执行命令、结果及覆盖范围。发现错误时保留能复现它的用例，注明修正原因；未执行的检查不写“通过”，本地验证不标为在线评测 AC。

构建后查看 `build/main.log` 中的缺字、字体替代、未定义引用、Overfull／Underfull 警告。维护源码、目录与讲解文件，生成的 PDF／aux／toc 等留在 build/，不手工修改。当前按约定不自动打开预览。

### 六、内容与排版约定

纯定理、模型和 STL 直接调用不建立空源码。教程讲解对外接口和简单调用；几何保留详细接口说明及逐函数用途注释。DP 优先写清状态、边界、转移、顺序和答案位置。

代码使用 Tab、同行左大括号和简短竞赛命名；有共享状态才用结构体，普通函数与 DP 不强制封装。图／树 DFS 用递归，算法所需点栈、单调栈正常保留。不批量格式化既有实现，不放 assert，输入前提写在说明中；指定保留的算法实现继续遵循仓库维护约定。

文档用 UTF-8。全局字体、颜色、间距与代码样式在 `config/style.tex` 调整，公共命令在 `config/commands.tex`；单个条目不要重复配置整套样式。`\InlineCode{...}` 可写下划线，仍须配对花括号，不能直接包含 `%`。

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
| 线段树分治／动态二分图 | `python scripts/check_data_structures.py --time-segment`，也含在数据结构 `--smoke` 与全书 `--full` 中 |
| 闭合子图／面积／插值卷积判素 | 分别用 check_graphs.py 的 `--closure`、check_geometry.py 的 `--area`、check_number_theory.py 的 `--additions` |
| 比赛脚本 | `python scripts/check_contest.py`；只查简版 run.sh 加 `--run-only`，Windows 需 Git Bash |

专项按实际覆盖范围记录，不把本地检查标注为在线评测 AC；历史结果保留在 [校验记录](docs/validation.md)。

几何专项包含近乎平行的半平面、极薄旋转区域、圆与多边形相切、大小圆求交及三维构造；整数／Fraction／100 位 Decimal、解析面积和变换性质分别核对。按小圆尺度检查面积，浮点默认阈值以 long double 至少 64 位有效二进制位为基准。逐项范围见 [几何专项复核](docs/geometry-review.md)。

## 提交与远程构建

远程为 [Personaowl/mtyTemplate](https://github.com/Personaowl/mtyTemplate)，分支 main。完成上述修改与检查后，在根目录执行：

```sh
git status --short
git diff --check
git diff
```

核对新增文件、改动范围和检查结果，确认没有遗漏源码、讲解或检查入口。然后暂存本次要上传的文件；若当前所有改动都属于这次更新，可以使用：

```sh
git add .
git diff --cached
git commit -m "Describe the handbook update"
git push origin main
```

提交说明改成实际更新内容，例如 `Add minimal rotation template`；只想提交部分修改时，将 `git add .` 换成具体文件路径。`git diff --cached` 用来查看本次提交真正包含的内容。

推送成功后打开 [GitHub Actions](https://github.com/Personaowl/mtyTemplate/actions)，找到本次提交对应的 **Build handbook**：工作流依次构建 PDF、编译全部模板与调用示例，再上传 `mtyTemplate-pdf`。成功后可下载压缩包中的 main.pdf；仓库上传的是可维护的源码与说明，build/ 中的 PDF 不提交。PDF 每次由工作流重新生成，Artifact 保留 30 天。

若失败，打开红色步骤查看首个实际错误：正文构建问题看 `.tex` 文件与行号；C++ 编译问题看模板和教程调用；新章未被检查时，回查 check_code.py 的登记。修复后重新提交、推送即可触发下一次构建。单看历史某次绿色运行，不能说明最新提交已通过。

多设备维护时，可在开始修改前、工作区干净时运行 `git pull --ff-only origin main` 同步远程。整体仓库尚未指定开源许可证，参考出处保留在条目注释中。
