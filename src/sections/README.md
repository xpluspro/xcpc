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

1. `DynamicProgramming`：动态规划
2. `String`：字符串
3. `NumberTheory`：数论（不含博弈论）
4. `Polynomial`：多项式与生成函数
5. `LinearAlgebra`：线性代数
6. `DataStructures`：数据结构
7. `Tree`：树上问题
8. `ComputationalGeometry`：计算几何
9. `GraphTheory`：图论
10. `OfflineAlgorithms`：离线算法（如 CDQ、莫队）
11. `OtherTools`：其它工具

工程根目录下的 `archive/` 是旧版未整理素材与历史模板的归档区，不参与
`main.tex` 的编译。需要启用其中的内容时，应先把素材移入对应版块的
`assets/`，再由该版块的 `index.tex` 引用。

从 `src/` 目录编译：

```bash
latexmk -xelatex -shell-escape main.tex
```
