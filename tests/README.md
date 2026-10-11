# 保存するテスト

ライブラリ本体と独立したC++23の実行プログラムを、`tests/カテゴリ/LibraryName.test.cpp` に置く。各ファイルは自身の `main()` を持ち、対象ソースを `#include "カテゴリ/LibraryName.cpp"` で読み込む。

配置はAIが [RepositoryLayout.md](../Rule/RepositoryLayout.md) に従って行う。木専用の本体は `tree/`、対応テストは `tests/tree/`。本体を移す場合はテストとincludeも同期する。共通チェックで対応テストの不足、場所の不一致、古いincludeを検出する。

`python3 tools/check_repository.py` は全 `*.test.cpp` を自動で検出し、各プログラムをビルドして実行する。通常実行では最適化と標準ライブラリの境界確認を使い、assertは有効。`--sanitizers` ではAddressSanitizerとUndefinedBehaviorSanitizerを追加する。失敗・コンパイルエラー・実行のタイムアウトは終了コード1となる。

## 現在の回帰テスト（18ライブラリ）

| 対象 | 独立した確認方法 | 壊れやすい条件 |
| --- | --- | --- |
| [UnionFind](../data_structure/UnionFind.cpp) | 追加した辺からBFSで毎回成分を再計算 | 空・1頂点、繰り返し結合、成分数・サイズ・groupsの整合 |
| [SegmentTree](../data_structure/SegmentTree.cpp) | 通常配列の和・逐次探索・文字列連結と比較 | 空区間、端点、非2冪、非可換演算の順序、max_right/min_left |
| [LazySegmentTree](../data_structure/LazySegmentTree.cpp) | 通常配列へ直接作用させて比較 | 区間と点への作用、代入との混在、作用合成の順序、空区間、境界探索 |
| [OrderedSet](../data_structure/OrderedSet.cpp) | std::setと比較 | 挿入・消去・順位・両方向のイテレータ、昇順／降順 |
| [RunLengthEncoding](../algorithm/RunLengthEncoding.cpp) | 元データへ復元して比較 | 空・繰り返し・最大連続区間、文字列と配列 |
| [ConvexPolygon](../geometry/ConvexPolygon.cpp) | 凸・凹・共線の既知形状を回転／反転 | 向き・共線・大きな座標と__int128 |
| [Dijkstra](../graph/Dijkstra.cpp) | 小グラフの単純パス全列挙 | 有向／無向、ゼロ辺、自己ループ、多重辺、未到達、経路復元 |
| [BellmanFord](../graph/BellmanFord.cpp) | 単純パス・単純サイクル全列挙 | 到達可能な負閉路と影響伝播、復元、long long端の中間計算 |
| [WarshallFloyd](../graph/WarshallFloyd.cpp) | 単純パス・単純サイクル全列挙 | 全点対の負閉路影響、復元、空グラフ、再構築 |
| [SCC](../graph/SCC.cpp) | 各頂点のBFSと相互到達性 | 強連結成分の一覧・個数・トポロジカルID、再構築 |
| [CycleDetection](../graph/CycleDetection.cpp) | Kahnの除去法／単純な成分管理 | 有向／無向、自己ループ、多重辺、辺と頂点の復元、キャッシュ更新 |
| [LCA](../tree/LCA.cpp) | 親を1つずつ辿り、パスを列挙 | 祖先・距離・全jump位置・範囲外、異なる根で再構築 |
| [TreeDiameter](../tree/TreeDiameter.cpp) | 全始点からの距離 | 正の重み、1頂点、端点・辺・頂点列の復元、再構築 |
| [MaxFlow](../graph/MaxFlow.cpp) | 全カット列挙 | 保存則、残余到達性、自己ループ、多重辺、再呼出し |
| [MinCostFlow](../graph/MinCostFlow.cpp) | 全辺流量を列挙して最小費用を比較 | slopeの全区間・圧縮、再呼出し、自己ループ、多重辺、LLONG_MAXまでの費用 |
| [ProjectSelection](../graph/ProjectSelection.cpp) | 全部分集合を列挙 | 利益・損失・ペナルティ・依存関係、選択結果の復元、空集合 |
| [ModInt](../math/ModInt.cpp) | 直接の整数計算と拡張Euclid | 3つの素数MOD、加減乗除・負数・累乗・入出力 |
| [Prime](../math/Prime.cpp) | 独立した600万までのふるい、因数と約数の復元 | 素数判定・素数計数・大きな合成数・最小の原始根 |

seedは各テストソース内の20261012に固定して再現性を保つ。全18の回帰テストは保存済みの入力範囲の確認であり、全入力・最大規模・全テンプレート型の正当性や性能を認定しない。

`tests/tools/test_snippets.py`はスニペットの展開文字列、solveの$0位置、文字としてのドル・バックスラッシュ、誤った冒頭行・重複名の検出を確認する。共通チェックはテンプレートと各ライブラリを組み合わせたC++23構文も確認する。

`tests/tools/test_repository_layout.py`はtreeと新カテゴリの自動検出、本体だけを移した場合の不足・取り残されたテスト、テストの古いinclude、カテゴリ外への本体配置の検出を確認する。

## 新規開発・修正時

対象に応じた基本・境界ケースと、必要な愚直比較・ランダム比較を追加する。実装と同じアルゴリズムを複製するだけの比較は避ける。対象のテンプレート引数を実際に使用する。計算量・数値範囲・最大制約の検証が必要なら別途実施して [作業記録](../records/DevelopmentLog.md) に範囲を記す。

`.test.cpp` は単独実行で検証が完了する回帰テストに使う。オンラインジャッジへの提出・AC・再ACは開発の必須工程に含めず、必要なC++23テストとGitHub Actionsの検証は継続する。実施したテストの範囲・結果と未確認事項は [DevelopmentLog.md](../records/DevelopmentLog.md) に記録する。
