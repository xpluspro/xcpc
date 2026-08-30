#!/usr/bin/env python3
"""Fail CI if a live handbook C++ snippet is not mapped to a test."""
from __future__ import annotations

import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parents[2]
SECTIONS = ROOT / "src" / "sections"
TESTS = ROOT / "tests"

# status: include | adapter | skip
# include: some test TU contains the path string
# adapter: Kind F/A wrapper, listed here
# skip: documented reason
MANIFEST = {
    "sections/ComputationalGeometry/assets/Geometry/8.1-declarations.cpp": "adapter: tests/prelude/geo_kernel.hpp",
    "sections/ComputationalGeometry/assets/Geometry/8.2-point-vector.cpp": "adapter: tests/prelude/geo_kernel.hpp",
    "sections/ComputationalGeometry/assets/Geometry/8.3-line.cpp": "adapter: tests/prelude/geo_kernel.hpp",
    "sections/ComputationalGeometry/assets/Geometry/8.4-circle.cpp": "include",
    "sections/ComputationalGeometry/assets/Geometry/AirportConstruction.cpp": "skip: polygon helper; no standalone oracle in v1",
    "sections/ComputationalGeometry/assets/Geometry/DelaunayTriangulation.cpp": "skip: large incremental triangulation, memory-pool fragment",
    "sections/ComputationalGeometry/assets/Geometry/Triangulation.cpp": "skip: ear clipping depends on global n,p[]",
    "sections/ComputationalGeometry/assets/Geometry/convex_findmax.cpp": "skip: requires prebuilt convex hull struct",
    "sections/ComputationalGeometry/assets/Geometry/dynamic_convex_hull.cpp": "skip: integer-only hull with extra globals",
    "sections/ComputationalGeometry/assets/Geometry/integral_hpi.cpp": "skip: integer HPI replacement helpers",
    "sections/ComputationalGeometry/assets/Geometry/三维几何.cpp": "skip: p3 kernel fragment",
    "sections/ComputationalGeometry/assets/Geometry/三维凸包.cpp": "skip: depends on 三维几何 p3",
    "sections/ComputationalGeometry/assets/Geometry/三角形.cpp": "include",
    "sections/ComputationalGeometry/assets/Geometry/凸包.cpp": "include",
    "sections/ComputationalGeometry/assets/Geometry/半平面交.cpp": "include",
    "sections/ComputationalGeometry/assets/Geometry/圆上整点.cpp": "include",
    "sections/ComputationalGeometry/assets/Geometry/圆并.cpp": "skip: event-queue circle union, large globals",
    "sections/ComputationalGeometry/assets/Geometry/多边形和圆的交.cpp": "skip: depends on line-circle helpers",
    "sections/ComputationalGeometry/assets/Geometry/多边形基础.cpp": "skip: polygon utilities, follow-up",
    "sections/ComputationalGeometry/assets/Geometry/旋转卡壳.cpp": "skip: rotating calipers, follow-up",
    "sections/ComputationalGeometry/assets/Geometry/最小覆盖球.cpp": "skip: Circle/Plane/p3 kernel + main()",
    "sections/ComputationalGeometry/assets/Geometry/最近点对.cpp": "skip: closest-pair divide-and-conquer fragment",
    "sections/ComputationalGeometry/assets/Geometry/经纬度求球面最短距离.cpp": "include",
    "sections/ComputationalGeometry/assets/Geometry/闵可夫斯基和.cpp": "skip: needs convex inputs and kernel operators",
    "sections/ComputationalGeometry/assets/Math/Simpson.cpp": "include",
    "sections/ComputationalGeometry/assets/yzh/circle_inversion.cpp": "skip: inversion helper on geo types",
    "sections/ComputationalGeometry/assets/yzh/golden_ternary.cpp": "skip: constexpr sqrt(LD) is not a constant expression under Apple libc++",
    "sections/DataStructures/assets/DataStructure/FHQ_treap.cpp": "include",
    "sections/DataStructures/assets/DataStructure/LCT.cpp": "adapter: tests/adapters/lct_basic.hpp first variant lines 1-34",
    "sections/DataStructures/assets/DataStructure/RangeBIT.cpp": "include",
    "sections/DataStructures/assets/DataStructure/bitset.cpp": "skip: incomplete knapsack fragment, no callable API",
    "sections/DataStructures/assets/DataStructure/hashmap.cpp": "include",
    "sections/DataStructures/assets/DataStructure/左偏树.cpp": "include",
    "sections/DataStructures/assets/DataStructure/李超线段树.cpp": "include",
    "sections/DataStructures/assets/DataStructure/线段树分裂.cpp": "skip: split fragment needs surrounding persistent segtree",
    "sections/DataStructures/assets/DataStructure/非递归线段树求最大值.cpp": "include",
    "sections/DataStructures/assets/Miscellany/RadixSort.cpp": "include",
    "sections/DynamicProgramming/assets/Miscellany/2D1D.cpp": "adapter: included inside knuth() in knuth_test.cpp",
    "sections/DynamicProgramming/assets/yzh/Subset Sum.cpp": "include",
    "sections/GraphTheory/assets/graph/2-sat.cpp": "include",
    "sections/GraphTheory/assets/graph/Blossom_Tree.cpp": "include",
    "sections/GraphTheory/assets/graph/CliqueCount.cpp": "adapter: dfs wrapped in clique_test.cpp / graph_guards.cpp",
    "sections/GraphTheory/assets/graph/Dinic.cpp": "include",
    "sections/GraphTheory/assets/graph/Hungarian.cpp": "include",
    "sections/GraphTheory/assets/graph/KM.cpp": "skip: dense KM needs n,m,lx,ly scaffolding",
    "sections/GraphTheory/assets/graph/MeanCycle.cpp": "skip: Karp DP fragment needs graph globals",
    "sections/GraphTheory/assets/graph/Tarjan.cpp": "skip: three concatenated algorithms, needs edge tables",
    "sections/GraphTheory/assets/graph/hk_skip2004.cpp": "include",
    "sections/GraphTheory/assets/graph/kosaraju.cpp": "include",
    "sections/GraphTheory/assets/graph/一般图最大匹配-shuffle.cpp": "skip: comment 正确性无法保证; not a hard CI oracle",
    "sections/GraphTheory/assets/graph/原始对偶费用流.cpp": "skip: MCMF needs full graph struct",
    "sections/GraphTheory/assets/graph/弦图.cpp": "skip: MCS fragment",
    "sections/GraphTheory/assets/graph/支配树.cpp": "skip: Lengauer-Tarjan needs DFS numbering prelude",
    "sections/GraphTheory/assets/graph/斯坦纳树.cpp": "skip: DP table fragment",
    "sections/GraphTheory/assets/graph/欧拉回路.cpp": "include",
    "sections/GraphTheory/assets/graph/环计数.cpp": "skip: 3/4-cycle counters need oriented adj",
    "sections/GraphTheory/assets/tbr/bipolar_orientation.cpp": "skip: st-numbering globals",
    "sections/GraphTheory/assets/yzh/Stoer-Wagner.cpp": "skip: main()+scanf template",
    "sections/LinearAlgebra/assets/Geometry/minnorm_gauss.cpp": "include",
    "sections/LinearAlgebra/assets/Math/Simplex.cpp": "include",
    "sections/NumberTheory/assets/Math/CRT_lbn.cpp": "include",
    "sections/NumberTheory/assets/Math/Factorial Mod.cpp": "include",
    "sections/NumberTheory/assets/Math/Miller Rabin And Pollard Rho.cpp": "include",
    "sections/NumberTheory/assets/Math/Pell方程.cpp": "include",
    "sections/NumberTheory/assets/Math/exgcd.cpp": "include",
    "sections/NumberTheory/assets/Math/min_25筛.cpp": "skip: inner engine needs pri/sta/g init",
    "sections/NumberTheory/assets/Math/平方剩余.cpp": "include",
    "sections/NumberTheory/assets/Math/扩展卢卡斯.cpp": "include",
    "sections/NumberTheory/assets/Math/直线下格点统计.cpp": "include",
    "sections/NumberTheory/assets/Math/解一元三次方程.cpp": "skip: file-scope fragment, Kind F later",
    "sections/NumberTheory/assets/Miscellany/LLFPM.cpp": "include",
    "sections/NumberTheory/assets/Miscellany/日期公式.cpp": "include",
    "sections/NumberTheory/assets/zjj/euclid.cpp": "skip: needs Val/ksm monoid",
    "sections/OfflineAlgorithms/assets/DataStructure/莫队二次离线.cpp": "skip: problem-specific fragment",
    "sections/Polynomial/assets/Math/FFT.cpp": "include",
    "sections/Polynomial/assets/Math/FWT.cpp": "include",
    "sections/Polynomial/assets/Math/MTT.cpp": "skip: uses GNU 1i imaginary literals incompatible with std::complex on AppleClang",
    "sections/Polynomial/assets/Math/NTT.cpp": "include",
    "sections/Polynomial/assets/Math/多点求值.cpp": "skip: needs poly_mod + ntt glue",
    "sections/Polynomial/assets/Math/多项式取模.cpp": "include",
    "sections/Polynomial/assets/Math/多项式运算.cpp": "include",
    "sections/Polynomial/assets/Math/快速线性递推-bostan-mori.cpp": "include",
    "sections/Polynomial/assets/Math/快速线性递推-多项式取模.cpp": "include",
    "sections/Polynomial/assets/Math/线性递推.cpp": "include",
    "sections/Polynomial/assets/tbr/fwt.cpp": "skip: k-ary FWT needs n,K,omega globals",
    "sections/RandomizationAndGameTheory/assets/Miscellany/hack.cpp": "skip: GCC pragmas + fread, compile-smoke not under ASan",
    "sections/String/assets/String/AC 自动机.cpp": "include",
    "sections/String/assets/String/EXKMP.cpp": "include",
    "sections/String/assets/String/KMP.cpp": "include",
    "sections/String/assets/String/Lyndon Word.cpp": "include",
    "sections/String/assets/String/Manacher.cpp": "include",
    "sections/String/assets/String/PAM.cpp": "include",
    "sections/String/assets/String/SA.cpp": "include",
    "sections/String/assets/String/SAM.cpp": "include",
    "sections/String/assets/String/SAMSA.cpp": "skip: DFS on SAM internals not in the file",
    "sections/String/assets/String/hash.cpp": "include",
    "sections/String/assets/String/广义SAM.cpp": "include",
    "sections/String/assets/String/最小表示法.cpp": "include",
    "sections/Tree/assets/tree/TreeHash.cpp": "include",
    "sections/Tree/assets/tree/虚树.cpp": "include",
}


def live_snippets():
    pat = re.compile(
        r"(?P<c>%\s*)?\\inputminted(?:\[[^\]]*\])?\{cpp\}\{(?P<path>[^}]+)\}"
    )
    live = []
    for idx in SECTIONS.glob("*/index.tex"):
        text = idx.read_text(encoding="utf-8")
        for m in pat.finditer(text):
            if m.group("c"):
                continue
            live.append(m.group("path"))
    return sorted(set(live))


def test_corpus():
    files = list(TESTS.rglob("*.cpp")) + list(TESTS.rglob("*.hpp")) + list(TESTS.rglob("*.h"))
    return "\n".join(p.read_text(encoding="utf-8", errors="replace") for p in files)


def main() -> int:
    live = live_snippets()
    body = test_corpus()
    missing_manifest = [p for p in live if p not in MANIFEST]
    extra = [p for p in MANIFEST if p not in live]
    errors = []
    if missing_manifest:
        errors.append("live snippets missing from MANIFEST:\n  " + "\n  ".join(missing_manifest))
    if extra:
        errors.append("MANIFEST entries not live in index.tex:\n  " + "\n  ".join(extra))
    for path, status in sorted(MANIFEST.items()):
        if path not in live:
            continue
        if status.startswith("include"):
            if path not in body:
                errors.append(f"include-status snippet not referenced in tests/: {path}")
        elif status.startswith("adapter"):
            pass
        elif status.startswith("skip"):
            pass
        else:
            errors.append(f"bad status for {path}: {status}")
    if errors:
        print("check_adapters FAILED:")
        for e in errors:
            print("-", e)
        return 1
    n_inc = sum(1 for v in MANIFEST.values() if v.startswith("include"))
    n_ad = sum(1 for v in MANIFEST.values() if v.startswith("adapter"))
    n_sk = sum(1 for v in MANIFEST.values() if v.startswith("skip"))
    print(f"OK: {len(live)} live snippets; include={n_inc} adapter={n_ad} skip={n_sk}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
