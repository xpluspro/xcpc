# 手册暴力对拍（不进入 CI）

本目录是 B7–B9、B11–B14、B24、B27–B36、B46、B51、B55 修复 PR 的**本地**对拍程序：每个 `dp_*.cpp` 用随机小数据 ≥10000 组、大数据 ≥1000 组对照暴力或另一套正确实现。

计算几何、数学与字符串相关对拍不在本 PR 中。

**GitHub Actions 不会编译或运行这里的任何文件。** CI 的 C++ 任务只处理 `src/sections` 与 `tools/test_*.cpp`；Python 任务只扫描 `src/sections/**/*.py`。

## 运行

需要较新的 GCC（含 `bits/stdc++.h`、`__int128`），例如 Homebrew `g++-16`。

```bash
cd tests/duipai
CXX=g++-16 ./run.sh
```

只跑某一个：

```bash
g++-16 -O2 -std=c++17 dp_lct.cpp -o /tmp/dp_lct && /tmp/dp_lct
```

产物写在 `build/`（已 gitignore），不要提交二进制。

## 与手册的对应

程序名 `dp_xxx` 对应本 PR 改过的活页片段。源码是自包含的：把手册逻辑抄进测试再和暴力比，不 `#include` 手册路径。
