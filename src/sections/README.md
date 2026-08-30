# 版块协作约定

`main.tex` 是稳定的装配入口，不再往里面添加具体知识点。

每个人只修改自己负责的版块目录。每个版块都是自包含的：

- `index.tex` 管理该版块的目录与排版顺序；
- `assets/` 保存该版块独占的代码、说明和脚本；
- 不允许从一个版块引用另一个版块的文件，也不引用 `archive/` 中的历史素材。

新增条目时，在本版块的 `index.tex` 中添加 `\subsection` 和对应的
`\input` / `\inputminted`，素材同时放入本版块的 `assets/`。顶层版块及装配顺序
统一由 `main.tex` 引用，日常内容修改不应触碰它。

目录与版块一一对应：

1. `OtherTools`：其它工具
2. `DynamicProgramming`：动态规划
3. `String`：字符串
4. `NumberTheory`：数论（不含博弈论）
5. `Polynomial`：多项式与生成函数
6. `LinearAlgebra`：线性代数
7. `DataStructures`：数据结构
8. `Tree`：树上问题
9. `ComputationalGeometry`：计算几何
10. `GraphTheory`：图论
11. `OfflineAlgorithms`：离线算法（如 CDQ、莫队）
12. `ContestGuidance`：统一置于文档末尾的比赛注意事项

工程根目录下的 `archive/` 是旧版未整理素材与历史模板的归档区，不参与
`main.tex` 的编译。需要启用其中的内容时，应先把素材移入对应版块的
`assets/`，再由该版块的 `index.tex` 引用。

从 `src/` 目录编译：

```bash
latexmk -xelatex -shell-escape main.tex
```

## CI 与语言版本

- C++ 默认必须兼容 C++17，并在 `-Wall -Wextra -Wshadow -Werror` 下零告警；
- Python 默认必须兼容 Python 3.10；
- 使用更高标准时，必须把版本标记写在文件第一行：

```cpp
// @requires: c++20
```

```python
# @requires: python3.11
```

CI 当前接受 C++17/C++20/C++23 和 Python 3.10--3.14。未标记文件一律按
C++17 或 Python 3.10 验证。C++ 素材通常是不能单独编译的模板片段，因此新增或
修改 `.cpp` 时，还必须把它纳入 `tools/test_*.cpp` 中的某个可编译测试单元；含
`main` 的完整程序可以直接接受验证。`archive/` 与 `SCL_others/` 是历史/外部资料，
不属于正式源码检查范围。
