人生不过醒时梦一场

一身胆色和热肠

若不衣锦不还乡

---
## Requirements:

### Package minted
```
pip install Pygments
```
Add Python Scripts where Pygments installed to your system path (e.g. `AppData\Roaming\Python\Python38\Scripts` on Windows, and `pygmentize.exe` should exist)

Add `-shell-escape` to your TeX arguments.

### Fonts

Main font: [思源宋体 Source Han Serif SC](https://github.com/adobe-fonts/source-han-serif/tree/release/)

Cover font: [Fusion Pixel 12px Proportional zh_hans](https://github.com/TakWolf/fusion-pixel-font)

* Linux User: It should be noted that there could be issues with `otf`s on Linux, and it seems `ttf` works better.

* Windows User: `Install for all users` is required for installing fonts.

## C++ tests

GoogleTest coverage for handbook snippets lives in `tests/`. GitHub Actions runs contract tests (with ASan/UBSan), assertion-guard death tests, and a snippet-to-test mapping check.

```bash
cmake -S tests -B build/tests -DXCPC_SANITIZE=OFF -DXCPC_DEATH_TESTS=OFF
cmake --build build/tests -j
ctest --test-dir build/tests --output-on-failure

cmake -S tests -B build/guards -DCMAKE_BUILD_TYPE=Debug -DXCPC_DEATH_TESTS=ON
cmake --build build/guards -j
ctest --test-dir build/guards --output-on-failure

python3 tests/sync/check_adapters.py
```