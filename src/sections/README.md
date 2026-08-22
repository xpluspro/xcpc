# 版块协作约定

`main.tex` 是稳定的装配入口，不再往里面添加具体知识点。

每个人只修改自己负责目录内的 `index.tex`；新增条目时，在该文件中添加
`\subsection` 和对应的 `\input` / `\inputminted`。顶层版块及装配顺序统一由
`main.tex` 引用，不需要为日常内容修改触碰它。

目录与版块一一对应：

1. `01-dynamic-programming`：动态规划
2. `02-string`：字符串
3. `03-number-theory`：数论（不含博弈论）
4. `04-polynomial`：多项式与生成函数
5. `05-linear-algebra`：线性代数
6. `06-data-structures`：数据结构
7. `07-tree`：树上问题
8. `08-computational-geometry`：计算几何
9. `09-graph-theory`：图论
10. `10-offline-algorithms`：离线算法（如 CDQ、莫队）
11. `11-randomization-game-theory`：随机化与博弈论

从 `src/` 目录编译：

```bash
latexmk -xelatex -shell-escape main.tex
```
