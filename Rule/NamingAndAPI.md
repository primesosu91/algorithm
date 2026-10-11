# NamingAndAPI — 命名・APIカタログ

## 1. 位置付けと確認版

- ソースコードの唯一の正式な原本: [primesosu91/algorithm](https://github.com/primesosu91/algorithm) の `main`。
- 調査基準: 2026-10-12（日本時間）、コミット [`eecc66b4fef2131aeedb8633d17763f45fe75c79`](https://github.com/primesosu91/algorithm/commit/eecc66b4fef2131aeedb8633d17763f45fe75c79)。当時の全18 `.cpp` を全文確認した。
- この資料は既存設計を正確に参照するためのカタログ。GitHubの最新実装と差異があれば実装を正とし、該当項目を更新する。共通方針は [開発規約](LibraryDevelopmentRules.md)、作業手順は [LibraryWorkflow.md](LibraryWorkflow.md)。
- この資料はAPIとソース版の参照先であり、テストの通過を認定するものではない。対象版の検証結果と未確認事項は [DevelopmentLog.md](../records/DevelopmentLog.md) とGitHub Actionsの該当実行履歴を確認する。オンラインACと採用状態の個別管理は必須工程に含めない。
- 一覧はソースに**明示された**コンストラクタ、デストラクタ、削除された操作、公開関数、friend関数、公開型・変数を対象とする。コンパイラが暗黙生成する特殊メンバやC++23が合成する候補は列挙しない。
- public の内部補助と通常の利用APIを区別する。公開変数への直接書込みや補助関数の単独実行が安全であることを意味しない。private のメンバは公開一覧に含めない。
- シグネチャの空白・改行のみを正規化し、テンプレート引数、既定引数、参照、const、friend、delete を保持する。コンストラクタの初期化リストは仕様説明に記す。

各項目のソースリンクは通常の `main` 参照。直下の `catalog-source` コメントにその項目の確認済みGit blob SHAを保持する。コメントも含めてソースが変わったら照合対象になる。全体の基準コミットは初回調査の記録であり、以後の更新では個別SHAと追記した確認コミットを使う。

追加確認: 2026-10-12、基準main `34eede7d6d376275e1f25bb93403e63b8abd9bc8` の全18ソースを全文確認。MinCostFlowのみ不具合修正を同時反映し、各catalog-sourceを実際のソースへ合わせた。

配置更新: 2026-10-12、基準main `416edcb1754776c006600bda50a7bcb46a5588a6` のLCA・TreeDiameterの本体と対応テストを読み、`graph/` から `tree/` へ移動。両本体の内容・公開API・blob SHAは維持し、一覧・ソースリンク・catalog-sourceのパスを更新した。全体のAPI再調査を行った更新ではない。分類の基準は [RepositoryLayout.md](RepositoryLayout.md)。

配置更新: 2026-10-12、基準main `ca75829056fbc9c6c6d927645dec8a1448fa7412` の全18ライブラリを `library/カテゴリ/` へ集約。一覧・ソースリンク・catalog-sourceのパスを更新し、全18の内容・公開API・blob SHAを維持した。API仕様を変更する更新ではない。

## 2. 全対象一覧

| カテゴリ | ファイル・ライブラリ | 主な公開型／関数 |
| --- | --- | --- |
| `algorithm` | [RunLengthEncoding](../library/algorithm/RunLengthEncoding.cpp) | `run_length_encoding (string / vector<T>)` |
| `data_structure` | [LazySegmentTree](../library/data_structure/LazySegmentTree.cpp) | `LazySegmentTree<T,op,e,F,mapping,composition,id>` |
| `data_structure` | [OrderedSet](../library/data_structure/OrderedSet.cpp) | `OrderedSet<T,Compare> / iterator` |
| `data_structure` | [SegmentTree](../library/data_structure/SegmentTree.cpp) | `SegmentTree<T,op,e>` |
| `data_structure` | [UnionFind](../library/data_structure/UnionFind.cpp) | `UnionFind` |
| `geometry` | [ConvexPolygon](../library/geometry/ConvexPolygon.cpp) | `Point / cross / is_convex` |
| `graph` | [BellmanFord](../library/graph/BellmanFord.cpp) | `BellmanFord<T>` |
| `graph` | [CycleDetection](../library/graph/CycleDetection.cpp) | `CycleDetection` |
| `graph` | [Dijkstra](../library/graph/Dijkstra.cpp) | `Dijkstra<T>` |
| `graph` | [MaxFlow](../library/graph/MaxFlow.cpp) | `MaxFlow / EdgeInfo` |
| `graph` | [MinCostFlow](../library/graph/MinCostFlow.cpp) | `MinCostFlow / EdgeInfo / FlowResult` |
| `graph` | [ProjectSelection](../library/graph/ProjectSelection.cpp) | `ProjectSelection / Penalty / FlowGraph` |
| `graph` | [SCC](../library/graph/SCC.cpp) | `SCC` |
| `graph` | [WarshallFloyd](../library/graph/WarshallFloyd.cpp) | `WarshallFloyd<T>` |
| `math` | [ModInt](../library/math/ModInt.cpp) | `ModInt<MOD>` |
| `math` | [Prime](../library/math/Prime.cpp) | `is_prime / factorize / divisors / prime_count / primitive_root` |
| `tree` | [LCA](../library/tree/LCA.cpp) | `LCA` |
| `tree` | [TreeDiameter](../library/tree/TreeDiameter.cpp) | `TreeDiameter` |

## 3. ライブラリ別の公開宣言と仕様

### RunLengthEncoding

ソース: [`library/algorithm/RunLengthEncoding.cpp`](../library/algorithm/RunLengthEncoding.cpp)

<!-- catalog-source: library/algorithm/RunLengthEncoding.cpp 2490c1cdd542e3b0eee31fc02180f7e0e72cfae2 -->

状態を持つ struct は使わず、通常関数のオーバーロードと関数テンプレートを提供する。空入力は空の列。入力順を保ち、隣接して等しい値だけをまとめる（ソートや全体の頻度集計ではない）。配列版は `!=` 比較と値のコピーが必要。連続数は `int` の範囲で扱う。

#### 通常関数

| 完全な公開シグネチャ | 引数・返り値・動作 |
| --- | --- |
| `vector<pair<char, int>> run_length_encoding(const string& s);` | 文字列 `s` を `{文字, 連続数}` の列として値で返す。 |
| `template<class T> vector<pair<T, int>> run_length_encoding(const vector<T>& values);` | 配列 `values` を `{値, 連続数}` の列として値で返す。型 `T` を推論できる。 |

### LazySegmentTree

ソース: [`library/data_structure/LazySegmentTree.cpp`](../library/data_structure/LazySegmentTree.cpp)

<!-- catalog-source: library/data_structure/LazySegmentTree.cpp 73ca088f5d8446329dd2a85ab82a41c15e94fa3b -->

`T/op/e` に加え、作用型 `F`、`mapping(F,T)`、`composition(F,F)`、`id()` を指定する。`composition(f,g)` は **g の後に f** を適用する合成。作用は集約値と整合し、`id()` は恒等作用であることが必要。区間長が必要なら `T` 側に `len` 等を持たせる。`mapping` に区間長を別引数で渡す設計ではない。
取得・探索も遅延伝播で内部配列を変更し得るため `const` ではない。`all_prod()` は遅延伝播しない。補助関数 `update/all_apply/push` は実装上 public のままだが、通常の要素操作APIとは分けて扱う。

公開型・名前空間の宣言（テンプレートを含む）:

```cpp
template<class T, T (*op)(T, T), T (*e)(), class F, T (*mapping)(F, T), F (*composition)(F, F), F (*id)()> struct LazySegmentTree;
```

#### LazySegmentTree

| 完全な公開シグネチャ | 引数・返り値・動作 |
| --- | --- |
| `void update(int k);` | 【公開された内部補助】内部ノード `k` を左右の子から再計算する。要素位置ではない。内部の整合性・有効な内部番号が前提。 |
| `void all_apply(int k, F f);` | 【公開された内部補助】内部ノード `k` の集約値に `f` を作用させ、内部ノードなら遅延作用を合成する。 |
| `void push(int k);` | 【公開された内部補助】内部ノード `k` の遅延作用を左右の子へ伝え、当該遅延作用を `id()` に戻す。 |
| `LazySegmentTree(int n);` | `n >= 0`。論理要素数 `n`、全要素を `e()` で初期化。内部の葉数 `size` は `n` 以上の2冪（最小1）。 遅延配列は `id()`。 |
| `LazySegmentTree(const vector<T>& v);` | `v` を初期配列として構築。論理要素数は `(int)v.size()`、余った葉は `e()`。 遅延配列は `id()`。 |
| `void set(int pos, T value);` | 位置 `pos` の値を `value` に置き換える。`0 <= pos < n`。加算ではない。 必要な遅延伝播を行う。 |
| `T get(int pos);` | 位置 `pos` の現在値を返す。`0 <= pos < n`。 必要な遅延伝播を行う。 |
| `T prod(int left, int right);` | `[left, right)` を元の順序で `op` 集約して返す。`0 <= left <= right <= n`、空区間は `e()`。 必要な遅延伝播を行う。 |
| `T all_prod();` | 全区間 `[0, n)` の集約値を返す。`n = 0` なら `e()`。 |
| `void apply(int pos, F f);` | 位置 `pos` の値に作用 `f` を適用する。`0 <= pos < n`。値の置換である `set` と区別する。 |
| `void apply(int left, int right, F f);` | 区間 `[left, right)` に作用 `f` を適用する。`0 <= left <= right <= n`。空区間は何もしない。 |
| `template<class G> int max_right(int left, G g);` | `g(prod(left, right))` が true の最大の `right` を返す。`0 <= left <= n`、`g(e()) == true` と区間伸長に対する単調性が必要。 遅延伝播を行う。 |
| `template<class G> int min_left(int right, G g);` | `g(prod(left, right))` が true の最小の `left` を返す。`0 <= right <= n`、`g(e()) == true` と区間伸長に対する単調性が必要。 遅延伝播を行う。 |

| 公開変数の宣言 | 意味 |
| --- | --- |
| `int n;` | 論理要素数。 |
| `int size;` | 内部の葉数（2冪、最小1）。 |
| `int log;` | 内部木の高さ。 |
| `vector<T> data;` | 1始まりの内部木の集約値。 |
| `vector<F> lazy;` | 内部ノードの未伝播作用。初期値は `id()`。 |

### OrderedSet

ソース: [`library/data_structure/OrderedSet.cpp`](../library/data_structure/OrderedSet.cpp)

<!-- catalog-source: library/data_structure/OrderedSet.cpp d5e69129b4e8361fc496f7c469fa2bf6fda4bf6b -->

`T` と比較型 `Compare = less<T>` を指定する、重複を持たない集合。厳密弱順序の比較関数が前提。順位・下限・上限は数値大小ではなく **Compare の順序** を使う。`less<T>` なら通常の昇順。構築時は空、乱数生成器は固定シード。
外側の `root/comp/rng`、`Node` と木操作は private。公開メンバ変数はない。コピー構築・コピー代入は禁止され、明示したムーブ操作もない。イテレータは読取専用で、要素を書き換えられない。消去した要素への参照・イテレータ、および破棄後の参照・イテレータは無効。

公開型・名前空間の宣言（テンプレートを含む）:

```cpp
template<class T, class Compare = less<T>> struct OrderedSet;
struct iterator;
```

#### OrderedSet

| 完全な公開シグネチャ | 引数・返り値・動作 |
| --- | --- |
| `OrderedSet(Compare comp = Compare());` | 比較オブジェクト `comp` を受け取り、空の集合を構築する。引数省略時は `Compare()`。 |
| `~OrderedSet();` | 保持する全ノードを解放する。 |
| `OrderedSet(const OrderedSet&) = delete;` | コピーコンストラクタは削除済み。呼び出せない。 |
| `OrderedSet& operator=(const OrderedSet&) = delete;` | コピー代入は削除済み。呼び出せない。 |
| `bool insert(const T& value);` | `value` を挿入する。比較上同値の要素が既にあると false、新規挿入で true。 |
| `bool erase(const T& value);` | `value` と比較上同値の要素を削除する。削除できたら true、なければ false。 |
| `bool contains(const T& value) const;` | `value` と比較上同値の要素があれば true。 |
| `int size() const;` | 集合全体の現在の要素数を返す。 |
| `bool empty() const;` | 空集合なら true。 |
| `const T& kth(int k) const;` | 比較順で0始まりの `k` 番目の値への const 参照を返す。`0 <= k < size()`。 |
| `int order_of(const T& value) const;` | 比較順で `value` より前の要素数を返す。`value` 自身の存在は不要。 |
| `iterator begin() const;` | 比較順の最初の要素へのイテレータ。空なら `end()`。 |
| `iterator end() const;` | この集合に属する終端イテレータを返す。 |
| `iterator find(const T& value) const;` | 比較上同値の要素へのイテレータ。見つからなければ `end()`。 |
| `iterator lower_bound(const T& value) const;` | 比較順で `value` より前ではない最初の要素。なければ `end()`。既定比較なら最小の `>= value`。 |
| `iterator upper_bound(const T& value) const;` | 比較順で `value` より後ろの最初の要素。なければ `end()`。既定比較なら最小の `> value`。 |

#### OrderedSet::iterator

| 完全な公開シグネチャ | 引数・返り値・動作 |
| --- | --- |
| `iterator();` | 未所属のイテレータを構築する（`node/owner` は null）。特定集合の `end()` と同じとは限らない。 |
| `const T& operator*() const;` | 現在の要素への const 参照を返す。終端・未所属では不可。 |
| `const T* operator->() const;` | 現在の要素への const ポインタを返す。終端・未所属では不可。 |
| `iterator& operator++();` | 比較順の次へ進み、自身への参照を返す。末尾要素からは `end()` へ。`++end()` は不可。 |
| `iterator operator++(int);` | 比較順の次へ進み、変更前のイテレータを値で返す。`int` は後置演算子の識別用。 |
| `iterator& operator--();` | 比較順の前へ戻り、自身への参照を返す。非空集合の `--end()` は可。`--begin()`、空集合の `--end()`、未所属では不可。 |
| `iterator operator--(int);` | 比較順の前へ戻り、変更前のイテレータを値で返す。`int` は後置演算子の識別用。 |
| `bool operator==(const iterator& other) const;` | 保持ノードと所属集合が両方同じなら true。 |
| `bool operator!=(const iterator& other) const;` | 等価でなければ true。 |

### SegmentTree

ソース: [`library/data_structure/SegmentTree.cpp`](../library/data_structure/SegmentTree.cpp)

<!-- catalog-source: library/data_structure/SegmentTree.cpp 03ff41bbb1f74eb970106545266be594a043dc13 -->

`T`、結合演算 `op(T,T)`、単位元生成 `e()` を関数ポインタのテンプレート引数で指定する。結合則と単位元の性質が必要。非可換演算でも左右の順序を維持する。公開APIは0始まり・半開区間。明示した全メンバ関数に `const` は付いていないが、取得・探索は配列を変更しない。

公開型・名前空間の宣言（テンプレートを含む）:

```cpp
template<class T, T (*op)(T, T), T (*e)()> struct SegmentTree;
```

#### SegmentTree

| 完全な公開シグネチャ | 引数・返り値・動作 |
| --- | --- |
| `SegmentTree(int n);` | `n >= 0`。論理要素数 `n`、全要素を `e()` で初期化。内部の葉数 `size` は `n` 以上の2冪（最小1）。 |
| `SegmentTree(const vector<T>& v);` | `v` を初期配列として構築。論理要素数は `(int)v.size()`、余った葉は `e()`。 |
| `void set(int pos, T value);` | 位置 `pos` の値を `value` に置き換える。`0 <= pos < n`。加算ではない。 |
| `T get(int pos);` | 位置 `pos` の現在値を返す。`0 <= pos < n`。 |
| `T prod(int left, int right);` | `[left, right)` を元の順序で `op` 集約して返す。`0 <= left <= right <= n`、空区間は `e()`。 |
| `T all_prod();` | 全区間 `[0, n)` の集約値を返す。`n = 0` なら `e()`。 |
| `template<class F> int max_right(int left, F f);` | `f(prod(left, right))` が true の最大の `right` を返す。`0 <= left <= n`、`f(e()) == true` と区間伸長に対する単調性が必要。 |
| `template<class F> int min_left(int right, F f);` | `f(prod(left, right))` が true の最小の `left` を返す。`0 <= right <= n`、`f(e()) == true` と区間伸長に対する単調性が必要。 |

| 公開変数の宣言 | 意味 |
| --- | --- |
| `int n;` | 論理要素数。 |
| `int size;` | 内部の葉数である2冪。`size()` というメンバ関数はない。 |
| `vector<T> data;` | 1始まりの内部木配列。要素 `pos` は葉 `size + pos` に対応。 |

### UnionFind

ソース: [`library/data_structure/UnionFind.cpp`](../library/data_structure/UnionFind.cpp)

<!-- catalog-source: library/data_structure/UnionFind.cpp 52213a1f40c4239411e43aacb768b17e299ee9f0 -->

代表的なスタイル基準。`n >= 0`。構築時は各頂点が独立し、`parent_or_size` は全て -1、`group_count = n`。再構築用の `init()` はない。頂点は `[0, n)`。
`leader` は経路圧縮を行うため、代表元・判定・サイズ・一覧の取得も内部状態を変え得る。`groups()` の頂点は各成分内で昇順、成分は代表元番号の昇順で並ぶ。代表元番号自体は結合順に依存する。

公開型・名前空間の宣言（テンプレートを含む）:

```cpp
struct UnionFind;
```

#### UnionFind

| 完全な公開シグネチャ | 引数・返り値・動作 |
| --- | --- |
| `UnionFind(int n);` | `n` 個の独立した頂点を構築する。コンストラクタに返り値はない。 |
| `int leader(int v);` | 頂点 `v` の代表元を返す。経路圧縮で親配列を変更する。 |
| `int merge(int u, int v);` | 頂点 `u, v` の成分をサイズに従って結合し、結合後の代表元を返す。既に同一ならその代表元。新たな結合で成分数を1減らす。 |
| `bool same(int u, int v);` | `u, v` が同じ成分なら true。内部で経路圧縮を行う。 |
| `int size(int v);` | `v` の所属成分の頂点数を返す。集合全体の要素数ではない。 |
| `int count();` | 現在の連結成分数を返す。 |
| `vector<vector<int>> groups();` | 全連結成分の頂点一覧を値で返す。親配列は経路圧縮され得る。 |

| 公開変数の宣言 | 意味 |
| --- | --- |
| `int n;` | 頂点数。 |
| `int group_count;` | 現在の連結成分数。 |
| `vector<int> parent_or_size;` | 負値は根の成分サイズの負数、非負値は親の頂点番号。直接変更すると不変条件を壊す。 |

### ConvexPolygon

ソース: [`library/geometry/ConvexPolygon.cpp`](../library/geometry/ConvexPolygon.cpp)

<!-- catalog-source: library/geometry/ConvexPolygon.cpp befd23138399b2f46a87732bf52ecea24e6537e3 -->

ファイル名は `ConvexPolygon.cpp` だが、同名の struct はない。公開型は `Point`、公開関数は `cross/is_convex`。`Point` には明示したコンストラクタがなく、`Point{x,y}` 等で座標を与える。既定初期化だけでは座標は初期化されない。
多角形の頂点を周上の順序で与え、**単純多角形**であることを前提とする。自己交差の検査はしない。連続3点の共線を許すため、厳密凸判定ではない。全点が一直線なら false。積と差が `__int128` に収まる必要がある。

公開型・名前空間の宣言（テンプレートを含む）:

```cpp
struct Point;
```

#### Point

| 公開変数の宣言 | 意味 |
| --- | --- |
| `long long x;` | x座標（`long long`）。 |
| `long long y;` | y座標（`long long`）。 |

#### 通常関数

| 完全な公開シグネチャ | 引数・返り値・動作 |
| --- | --- |
| `__int128 cross(const Point& a, const Point& b, const Point& c);` | 点 `a,b,c` から `(b-a) × (c-a)` を `__int128` で返す。正負は回転方向、0は共線。 |
| `bool is_convex(const vector<Point>& polygon);` | `polygon.size() >= 3`。単純多角形で回転方向が一貫し、非共線の3点を含むなら true。 |

### BellmanFord

ソース: [`library/graph/BellmanFord.cpp`](../library/graph/BellmanFord.cpp)

<!-- catalog-source: library/graph/BellmanFord.cpp 604849c233489ba68cfd05b4161d08b9fc911b58 -->

`T` はコスト・距離型。`n >= 0`、有向が既定。負辺を許す。到達性は `reached` で別管理し、INF 定数を持たない。有限な最短距離が `T` の範囲に収まることが前提。8バイト以下の整数型の作業距離には `__int128` を使う。
辺IDは追加順の0始まり。無向時は逆辺を同一IDで `edges` に格納する。構築前の結果配列は空、構築後は未到達・負閉路影響のある距離値を有効な距離として扱わない。辺追加で `built=false`。

公開型・名前空間の宣言（テンプレートを含む）:

```cpp
template<class T> struct BellmanFord;
struct Edge;
```

#### BellmanFord

| 完全な公開シグネチャ | 引数・返り値・動作 |
| --- | --- |
| `BellmanFord(int n, bool directed = true);` | 頂点数 `n`、有向フラグ `directed` で空グラフを構築。結果は未構築。 |
| `int add_edge(int from, int to, T cost);` | `from -> to` にコスト `cost` の辺を追加し、辺IDを返す。無向時は逆辺も同一IDで追加。 |
| `void build(int start);` | 始点 `start` からの距離・到達性・負閉路の影響・復元情報を再計算する。 |
| `T dist(int v) const;` | `v` の距離値を返す。`reachable(v) && !is_negative(v)` の場合にだけ意味がある。この条件自体は dist 内で assert されない。 |
| `bool reachable(int v) const;` | 始点から `v` に到達可能なら true。負閉路の影響があっても到達性は true。 |
| `bool is_negative(int v) const;` | 始点から到達可能な負閉路から `v` に到達でき、距離が確定しないなら true。 |
| `bool has_negative_cycle() const;` | 始点から到達可能な負閉路があれば true。グラフ全体の未到達部分の負閉路検査ではない。 |
| `vector<int> get_path(int v) const;` | 始点から `v` までの頂点列を値で返す。未到達・負閉路影響ありなら空、有効な始点自身なら1頂点。 |
| `vector<int> get_path_edges(int v) const;` | 同経路の辺ID列を値で返す。未到達・負閉路影響あり・有効な始点自身なら空。 |

| 公開変数の宣言 | 意味 |
| --- | --- |
| `int n;` | 頂点数。 |
| `bool directed;` | 有向かどうか。 |
| `int edge_count;` | 追加した論理辺数。 |
| `vector<Edge> edges;` | 全有向辺の配列。無向時は論理辺1本に対して2件。`edges()` 関数ではない。 |
| `vector<T> distance;` | 有効な頂点の有限最短距離。 |
| `vector<bool> reached;` | 始点から到達済みか。 |
| `vector<bool> negative;` | 負閉路の影響があるか。 |
| `vector<int> prev;` | 復元用の直前頂点（なければ -1）。 |
| `vector<int> prev_edge;` | 復元用の直前辺ID（なければ -1）。 |
| `bool built;` | 構築結果の有効フラグ。 |
| `bool negative_cycle;` | 始点から到達可能な負閉路の有無。 |

#### BellmanFord::Edge

| 公開変数の宣言 | 意味 |
| --- | --- |
| `int from;` | 始点。 |
| `int to;` | 終点。 |
| `T cost;` | コスト。 |
| `int id;` | 論理辺ID。 |

### CycleDetection

ソース: [`library/graph/CycleDetection.cpp`](../library/graph/CycleDetection.cpp)

<!-- catalog-source: library/graph/CycleDetection.cpp c5f5544dc440c301083a3e1a402b0fe69289e48b -->

`n >= 0`。`directed` は **省略できない**。無向時は1回の add_edge で両向きを同一IDで登録する。判定・復元を初めて呼んだ際に遅延探索し、最初に見つけた1つのサイクルを保持する。辺追加後は検索キャッシュを無効化し、頂点列・辺列を消す。
サイクルの頂点列は始点を末尾に重複させない。頂点数と辺数は同じで、辺 i は頂点 i から頂点 `(i+1)%m` に対応する。自己ループ・無向の平行辺も辺IDで識別する。

公開型・名前空間の宣言（テンプレートを含む）:

```cpp
struct CycleDetection;
```

#### CycleDetection

| 完全な公開シグネチャ | 引数・返り値・動作 |
| --- | --- |
| `CycleDetection(int n, bool directed);` | 頂点数 `n` と必須の有向フラグ `directed` で空グラフを構築する。 |
| `int add_edge(int u, int v);` | 頂点 `u -> v` の辺を追加し、0始まりの論理辺IDを返す。無向時は両向き。探索結果を無効化。 |
| `bool has_cycle();` | 必要なら探索を行い、サイクルがあれば true。内部状態を変更し得る。 |
| `vector<int> get_cycle_vertices();` | 必要なら探索してサイクルの頂点列を値で返す。始点の重複なし。なければ空。 |
| `vector<int> get_cycle_edges();` | 同じサイクルの辺ID列を値で返す。最後の辺は始点へ戻る。なければ空。 |

| 公開変数の宣言 | 意味 |
| --- | --- |
| `int n;` | 頂点数。 |
| `bool directed;` | 有向かどうか。 |
| `int edge_count;` | 追加した論理辺数。 |
| `vector<vector<pair<int, int>>> graph;` | 隣接 `{行き先, 辺ID}`。 |
| `bool searched;` | 探索済みか。 |
| `bool cycle_exists;` | 最後の探索でサイクルがあったか。辺追加直後は古い値が残り得るため、直接参照より has_cycle を使用する。 |
| `vector<int> cycle_vertices;` | 保持したサイクルの頂点列。 |
| `vector<int> cycle_edges;` | 保持したサイクルの辺ID列。 |

### Dijkstra

ソース: [`library/graph/Dijkstra.cpp`](../library/graph/Dijkstra.cpp)

<!-- catalog-source: library/graph/Dijkstra.cpp 8f95b4587063347fdf154fcb05f19f19aa8550e5 -->

`T` は距離・コストの型。`n >= 0`、有向が既定。辺は非負で `cost < INF`、有限な最短距離も `INF` 未満が前提。`INF` は static constexpr で `numeric_limits<T>::max()/2`。
`add_edge` の辺IDは追加順の0始まり。無向時は1回の追加で両向きに格納し、同一IDを共有する。`build(start)` 後に参照し、辺を追加すると `built=false` になり再構築が必要。初期距離は INF、親は -1、始点は -1。

公開型・名前空間の宣言（テンプレートを含む）:

```cpp
template<class T> struct Dijkstra;
struct Edge;
```

#### Dijkstra

| 完全な公開シグネチャ | 引数・返り値・動作 |
| --- | --- |
| `Dijkstra(int n, bool directed = true);` | 頂点数 `n` と有向フラグ `directed` で空グラフを構築する。 |
| `int add_edge(int from, int to, T cost);` | `from -> to` のコスト `cost` の辺を追加し、辺IDを返す。無向時は逆向きも追加。構築済み結果を無効化。 |
| `void build(int start);` | 始点 `start` からの距離と復元用親を再計算する。戻り値はない。 |
| `T dist(int v) const;` | 構築時の始点から `v` までの距離を返す。到達不能なら INF。 |
| `bool reachable(int v) const;` | 構築時の始点から `v` に到達可能なら true。 |
| `vector<int> get_path(int v) const;` | 始点から `v` までの頂点列を値で返す。到達不能なら空、始点自身なら `{start}`。 |
| `vector<int> get_path_edges(int v) const;` | 同じ経路の辺ID列を値で返す。到達不能・始点自身なら空。 |

| 公開変数の宣言 | 意味 |
| --- | --- |
| `static constexpr T INF = numeric_limits<T>::max() / 2;` | 未到達用の番兵定数。 |
| `int n;` | 頂点数。 |
| `bool directed;` | 有向かどうか。 |
| `int edge_count;` | 追加した論理辺の個数。 |
| `vector<vector<Edge>> graph;` | 隣接リスト。無向時は1辺を両向きに格納。 |
| `bool built;` | 構築結果が有効か。 |
| `int start;` | 最後に構築した始点。 |
| `vector<T> distance;` | 始点からの最短距離配列。 |
| `vector<int> parent_vertex;` | 復元用の直前頂点（なければ -1）。 |
| `vector<int> parent_edge;` | 復元用の直前辺ID（なければ -1）。 |

#### Dijkstra::Edge

| 公開変数の宣言 | 意味 |
| --- | --- |
| `int to;` | 行き先。 |
| `T cost;` | 辺のコスト。 |
| `int id;` | 論理辺ID。 |

### LCA

ソース: [`library/tree/LCA.cpp`](../library/tree/LCA.cpp)

<!-- catalog-source: library/tree/LCA.cpp 9cc01adcd28abd1c975ca854751cb0bce9b80cc1 -->

`n > 0` の **連結な無向木**を前提とする。重みは扱わず距離は辺数。1回の add_edge で両向きを登録する。構築時は depth/parent が -1。辺追加後は同じ根でも build を再実行する。
`built` フラグはない。各取得関数は対象の深さが -1 でないこと等を assert するが、辺追加による古い結果の無効化・木の妥当性検査は行わない。森・閉路付きグラフを一般に処理できる設計ではない。

公開型・名前空間の宣言（テンプレートを含む）:

```cpp
struct LCA;
```

#### LCA

| 完全な公開シグネチャ | 引数・返り値・動作 |
| --- | --- |
| `LCA(int n);` | 頂点数 `n` を受け取り、祖先表のサイズを確保する。木の辺を追加してから build が必要。 |
| `void add_edge(int u, int v);` | 頂点 `u,v` 間に無向辺を登録。辺IDは返さない。 |
| `void build(int root);` | 根 `root` から深さ・ダブリング祖先表を再計算。根は必須引数。 |
| `int kth_ancestor(int v, int k) const;` | `v` から親方向へ `k >= 0` 辺進んだ祖先を返す。`k=0` なら v、深さ超過なら -1。 |
| `int lca(int u, int v) const;` | 頂点 `u,v` の最小共通祖先を返す。 |
| `int distance(int u, int v) const;` | 頂点 `u,v` 間の辺数を返す。`dist` という名前ではない。 |
| `int jump(int u, int v, int k) const;` | パス `u -> v` 上を `u` から `k >= 0` 辺進んだ頂点。`k=0` なら u、全距離なら v、超過なら -1。 |

| 公開変数の宣言 | 意味 |
| --- | --- |
| `int n;` | 頂点数。 |
| `int log;` | ダブリング表の段数。 |
| `vector<vector<int>> graph;` | 無向木の隣接頂点一覧。 |
| `vector<vector<int>> parent;` | parent[k][v] は v の 2^k 個上の祖先。なければ -1。 |
| `vector<int> depth;` | 根からの辺数。未構築は -1。 |

### MaxFlow

ソース: [`library/graph/MaxFlow.cpp`](../library/graph/MaxFlow.cpp)

<!-- catalog-source: library/graph/MaxFlow.cpp 0efd635e006549d72006e2f61078699e160dc8e5 -->

`n >= 0`、非負容量の有向ネットワーク。構築時は空の残余グラフ。追加順の0始まりの辺番号を元の辺に割り当てる。逆辺は補助の残余辺で、元の辺一覧に含まれない。自己ループ用の逆辺位置補正がある。
`flow` は残余グラフを変更し、返すのは **その呼出しで追加できた流量**。累計は各辺の get_edge 等から確認する。構築済みフラグ・reset API・流量上限を指定するオーバーロードはない。始点終点は異なり、流量合計が `long long` に収まる必要がある。
`min_cut/source_side` を最小カットとして読むのは、同じ始点・終点について最大流を最後まで流した後。通常は現在の残余到達性を表す。

公開型・名前空間の宣言（テンプレートを含む）:

```cpp
struct MaxFlow;
struct EdgeInfo;
struct Edge;
```

#### MaxFlow

| 完全な公開シグネチャ | 引数・返り値・動作 |
| --- | --- |
| `MaxFlow(int n);` | 頂点数 `n` の空の残余ネットワークを構築する。 |
| `int add_edge(int from, int to, long long capacity);` | 有向辺 `from -> to` と非負容量 `capacity` を登録し、元の辺番号を返す。 |
| `long long flow(int source, int sink);` | 現在の残余グラフで `source` から `sink` へ最大限追加で流し、追加流量を返す。`source != sink`。 |
| `EdgeInfo get_edge(int edge_id) const;` | 元の辺 `edge_id` の始点・終点・元容量・現在流量を EdgeInfo として値で返す。 |
| `vector<EdgeInfo> edges() const;` | add_edge の追加順で、全ての元の辺の EdgeInfo を値で返す。 |
| `vector<bool> min_cut(int source) const;` | 現在の残余グラフで `source` から到達可能な各頂点の bool 配列を返す。 |
| `vector<int> source_side(int source) const;` | min_cut が true の頂点番号を昇順に列挙して返す。 |

| 公開変数の宣言 | 意味 |
| --- | --- |
| `int n;` | 頂点数。 |
| `vector<vector<Edge>> graph;` | 残余隣接リスト。Edge.capacity は現在の残余容量。 |
| `vector<pair<int, int>> edge_positions;` | 元の辺番号から `{始点, graph 内の位置}` への対応。 |
| `vector<int> level;` | Dinic のレベル。 |
| `vector<int> current_edge;` | Dinic の次に調べる辺位置。 |

#### MaxFlow::EdgeInfo

| 公開変数の宣言 | 意味 |
| --- | --- |
| `int from;` | 元の辺の始点。 |
| `int to;` | 元の辺の終点。 |
| `long long capacity;` | 元の容量（順辺残余容量＋逆辺残余容量）。 |
| `long long flow;` | 現在の流量（逆辺残余容量）。 |

#### MaxFlow::Edge

| 公開変数の宣言 | 意味 |
| --- | --- |
| `int to;` | 残余辺の行き先。 |
| `long long capacity;` | 残余容量。EdgeInfo.capacity とは意味が異なる。 |
| `int reverse_index;` | 相手側の隣接リストにある逆辺の添字。 |

### MinCostFlow

ソース: [`library/graph/MinCostFlow.cpp`](../library/graph/MinCostFlow.cpp)

<!-- catalog-source: library/graph/MinCostFlow.cpp eb45f905e889965b10c6730c150ecd3635816d7a -->

確認版: 2026-10-12、基準コミット `34eede7d6d376275e1f25bb93403e63b8abd9bc8` に対する今回の修正ソース。上記blob SHAで特定する。

`n >= 0` の有向ネットワーク。`add_edge` は `flow/slope` を初めて呼ぶ **前のみ**で、元の容量・単位コストはともに非負。逆辺のコストは負になる。元の辺番号は追加順の0始まり。
`flow/slope` は残余グラフを変更する。戻り値はその呼出しの追加流量・追加費用で、過去の累計ではない。`slope` は `{0,0}` を先頭とし、同じ限界費用の連続区間を圧縮する。再呼出し時、既存フローがあれば現在の残余グラフでポテンシャルを初期化する。
`source != sink`、`flow_limit >= 0`。流量・費用・途中計算は `long long` の表現可能範囲と実装のassertを満たす必要がある。距離の初期値は `max()/4` だが、到達性は別のフラグで管理し、この値以上の有効な距離も扱う。任意の始点終点変更後の負閉路を一般に処理する保証は確認していない。

公開型・名前空間の宣言（テンプレートを含む）:

```cpp
struct MinCostFlow;
struct EdgeInfo;
struct FlowResult;
struct Edge;
```

#### MinCostFlow

| 完全な公開シグネチャ | 引数・返り値・動作 |
| --- | --- |
| `MinCostFlow(int n);` | 頂点数 `n` で空の残余ネットワークを構築。`started/has_flow` は false。 |
| `int add_edge(int from, int to, long long capacity, long long cost);` | `from -> to` に非負容量 `capacity`、非負単位コスト `cost` の辺を登録し、元の辺番号を返す。最初の flow/slope より前のみ。 |
| `vector<FlowResult> slope(int source, int sink, long long flow_limit);` | `source -> sink` に上限 `flow_limit` まで追加で流し、追加流量と最小追加費用の折れ点を FlowResult 列で返す。先頭 `{0,0}`、同じ限界費用をまとめる。 |
| `FlowResult flow(int source, int sink, long long flow_limit);` | 同じ引数の slope の最終折れ点を返す。要求上限に達しない場合は実際に流せた量。追加できなければ `{0,0}`。 |
| `EdgeInfo get_edge(int edge_id) const;` | 元の辺 `edge_id` の始点・終点・元容量・現在流量・単位コストを値で返す。 |
| `vector<EdgeInfo> edges() const;` | 追加順で全ての元の辺の情報を値で返す。 |

| 公開変数の宣言 | 意味 |
| --- | --- |
| `int n;` | 頂点数。 |
| `vector<vector<Edge>> graph;` | コスト付き残余隣接リスト。 |
| `vector<pair<int, int>> edge_positions;` | 元の辺番号から `{始点, graph 内位置}` への対応。 |
| `vector<long long> potential;` | 最短路計算用のポテンシャル。 |
| `vector<long long> distance;` | 最短路計算用の作業距離。公開の dist API ではない。 |
| `vector<int> previous_vertex;` | 増加路の直前頂点。 |
| `vector<int> previous_edge;` | 増加路の直前辺の隣接リスト添字（元の辺IDではない）。 |
| `bool started;` | flow/slope を1回でも呼んだか。上限0でも true になる。 |
| `bool has_flow;` | 正の追加流量を1度でも流したか。 |

#### MinCostFlow::EdgeInfo

| 公開変数の宣言 | 意味 |
| --- | --- |
| `int from;` | 元の辺の始点。 |
| `int to;` | 元の辺の終点。 |
| `long long capacity;` | 元の容量。 |
| `long long flow;` | 現在の流量。 |
| `long long cost;` | 元の辺の単位コスト（総費用ではない）。 |

#### MinCostFlow::FlowResult

| 公開変数の宣言 | 意味 |
| --- | --- |
| `long long flow;` | その呼出しでの追加流量。slope では折れ点の追加流量。 |
| `long long cost;` | その呼出しでの追加総費用。単位コストではない。 |

#### MinCostFlow::Edge

| 公開変数の宣言 | 意味 |
| --- | --- |
| `int to;` | 残余辺の行き先。 |
| `long long capacity;` | 残余容量。 |
| `long long cost;` | 残余辺の単位コスト。逆辺では負。 |
| `int reverse_index;` | 逆辺の隣接リスト添字。 |

### ProjectSelection

ソース: [`library/graph/ProjectSelection.cpp`](../library/graph/ProjectSelection.cpp)

<!-- catalog-source: library/graph/ProjectSelection.cpp 087135952c0e40cdcfcd243c196256488ebf282d -->

`n >= 0`。各要素を選ぶ／選ばない最適化で、初期利益は全て0。追加APIは利益を加算し、ペナルティ・依存を蓄積する。`solve()` は **1回のみ**、実行後の追加も不可。`selected()` は solve 後のみ。
利益・ペナルティ容量の集計、依存用容量（有限容量合計＋1）と結果は実装の `long long` 範囲条件を満たす必要がある。内部で Dinic を実装しており、外部の MaxFlow.cpp に依存しない。
`Penalty` と `FlowGraph` は public のネスト型。後者は内部実装用で、独立した利用対象のライブラリとして扱わない。

公開型・名前空間の宣言（テンプレートを含む）:

```cpp
struct ProjectSelection;
struct Penalty;
struct FlowGraph;
struct Edge;
```

#### ProjectSelection

| 完全な公開シグネチャ | 引数・返り値・動作 |
| --- | --- |
| `ProjectSelection(int n);` | 要素数 `n` で利益0・制約なし・未計算の状態を構築する。 |
| `void add_profit(int v, long long value);` | 要素 `v` を選んだ時の利益に `value` を加算する。負値は損失。solve 前のみ。 |
| `void add_penalty(int u, int v, long long penalty);` | `u` を選び `v` を選ばない場合に非負の `penalty` を課す制約を追加する。solve 前のみ。 |
| `void add_dependency(int u, int v);` | `u` を選ぶなら `v` も選ぶ、という依存制約を追加する。向き・引数順を逆にしない。solve 前のみ。 |
| `long long solve();` | 全制約を満たす最大利益を `long long` で返し、選択結果を保存する。再呼出し不可。 |
| `vector<bool> selected() const;` | 最大利益を達成する選択の1つを bool 配列のコピーで返す。solve 後のみ。 |

| 公開変数の宣言 | 意味 |
| --- | --- |
| `int n;` | 要素数（公開頂点は `[0,n)`）。 |
| `vector<long long> profit;` | 要素ごとの選択時利益の累積値。 |
| `vector<Penalty> penalties;` | 条件付きペナルティの一覧。 |
| `vector<pair<int, int>> dependencies;` | `{u,v}` は u を選ぶなら v も選ぶ依存。 |
| `bool solved;` | solve を実行したか。 |
| `long long answer;` | solve で求めた最大利益。初期0。 |
| `vector<bool> selected_result;` | solve で得た選択結果。初期は空。 |

#### ProjectSelection::Penalty

| 公開変数の宣言 | 意味 |
| --- | --- |
| `int from;` | 選択側の条件 u。 |
| `int to;` | 非選択側の条件 v。 |
| `long long penalty;` | その条件で課す非負ペナルティ。 |

#### ProjectSelection::FlowGraph

| 完全な公開シグネチャ | 引数・返り値・動作 |
| --- | --- |
| `FlowGraph(int n);` | 【公開された内部型】頂点数 `n >= 0` で空の残余ネットワークを構築する。 |
| `void add_edge(int from, int to, long long capacity);` | 【内部型API】有向辺 `from -> to` と非負容量 capacity を追加。MaxFlow.add_edge と異なり返り値は void。 |
| `long long flow(int source, int sink);` | 【内部型API】残余ネットワークで source から sink に今回追加できる最大流量を返す。`source != sink`。 |
| `vector<bool> min_cut(int source) const;` | 【内部型API】残余ネットワークで source から到達可能な bool 配列を返す。 |

| 公開変数の宣言 | 意味 |
| --- | --- |
| `int n;` | 内部ネットワークの頂点数。solve 内では n+2。 |
| `vector<vector<Edge>> graph;` | 残余隣接リスト。 |
| `vector<int> level;` | Dinic のレベル。 |
| `vector<int> current_edge;` | Dinic の次に調べる辺位置。 |

#### ProjectSelection::FlowGraph::Edge

| 公開変数の宣言 | 意味 |
| --- | --- |
| `int to;` | 残余辺の行き先。 |
| `long long capacity;` | 残余容量。 |
| `int reverse_index;` | 逆辺の隣接リスト添字。 |

### SCC

ソース: [`library/graph/SCC.cpp`](../library/graph/SCC.cpp)

<!-- catalog-source: library/graph/SCC.cpp a9cf9244049a055edcbaa287c7d3c8d0573cabb2 -->

`n >= 0` の有向グラフ。構築時は辺なし、`component_ids` は -1。辺追加後は再構築する。構築後の成分IDは0始まりのトポロジカル順で、異なる成分間の `u -> v` では `component_id(u) < component_id(v)`。成分内の頂点順は探索順であり昇順保証ではない。

公開型・名前空間の宣言（テンプレートを含む）:

```cpp
struct SCC;
```

#### SCC

| 完全な公開シグネチャ | 引数・返り値・動作 |
| --- | --- |
| `SCC(int n);` | 頂点数 `n` の空の有向グラフを構築する。 |
| `void add_edge(int from, int to);` | 有向辺 `from -> to` を元グラフ・逆グラフに登録し、構築結果を無効化。辺IDは返さない。 |
| `void build();` | 強連結成分を再計算し、IDをトポロジカル順に付ける。 |
| `int component_id(int v) const;` | 頂点 `v` の所属する強連結成分IDを返す。構築後のみ有効。 |
| `int count() const;` | 強連結成分数を返す。構築後のみ有効。 |
| `const vector<vector<int>>& groups() const;` | 成分ID順の頂点一覧への const 参照を返す。構築後のみ有効。参照先の内容は再buildで変わり得る。 |

| 公開変数の宣言 | 意味 |
| --- | --- |
| `int n;` | 頂点数。 |
| `vector<vector<int>> graph;` | 元の有向グラフ。 |
| `vector<vector<int>> reverse_graph;` | 全辺を逆向きにしたグラフ。 |
| `bool built;` | 構築結果が有効か。 |
| `vector<int> component_ids;` | 各頂点の成分ID。 |
| `vector<vector<int>> component_groups;` | 成分IDごとの頂点一覧。 |

### TreeDiameter

ソース: [`library/tree/TreeDiameter.cpp`](../library/tree/TreeDiameter.cpp)

<!-- catalog-source: library/tree/TreeDiameter.cpp af703fa4be1ee9dc465e4b63bf9e15f076a578a5 -->

`n >= 1` の **連結な無向木**。重みは正、パス重み和は `long long` の範囲が前提。辺IDは追加順の0始まり、両向きで同一ID。add_edge の返り値はどちらも void。辺追加後は `built=false`、取得前に build。
単一頂点の場合は長さ0・端点 `{0,0}`・頂点列 `{0}`・辺列は空。頂点列は端点 first から second の順で、辺列の長さは頂点列の長さより1少ない。複数の直径のうち選ばれる1つの端点順は保証しない。

公開型・名前空間の宣言（テンプレートを含む）:

```cpp
struct TreeDiameter;
struct Edge;
```

#### TreeDiameter

| 完全な公開シグネチャ | 引数・返り値・動作 |
| --- | --- |
| `TreeDiameter(int n);` | 頂点数 `n` の空グラフを構築する。直径の結果は未構築。 |
| `void add_edge(int u, int v);` | 頂点 `u,v` 間に重み1の無向辺を追加する。 |
| `void add_edge(int u, int v, long long weight);` | 頂点 `u,v` 間に正の重み `weight` の無向辺を追加する。 |
| `void build();` | 木の直径長・端点・頂点列・辺列を再計算する。 |
| `long long diameter_length() const;` | 直径の重み和を `long long` で返す。 |
| `pair<int, int> diameter_endpoints() const;` | 直径の端点を `{始点, 終点}` として値で返す。 |
| `const vector<int>& diameter_vertices() const;` | 直径上の頂点列への const 参照を返す。再buildで内容が変わり得る。 |
| `const vector<int>& diameter_edges() const;` | 直径上の辺ID列への const 参照を返す。再buildで内容が変わり得る。 |

| 公開変数の宣言 | 意味 |
| --- | --- |
| `int n;` | 頂点数。 |
| `vector<vector<Edge>> graph;` | 無向辺を両方向に持つ隣接リスト。 |
| `int edge_count;` | 追加した論理辺数。 |
| `bool built;` | 構築結果が有効か。 |
| `long long diameter_dist;` | 直径長。 |
| `pair<int, int> diameter_ends;` | 直径の両端点。 |
| `vector<int> diameter_path_vertices;` | 直径の頂点列。 |
| `vector<int> diameter_path_edges;` | 直径の辺ID列。 |
| `vector<int> parent;` | 復元用の親頂点。 |
| `vector<int> parent_edge;` | 復元用の親辺ID。 |

#### TreeDiameter::Edge

| 公開変数の宣言 | 意味 |
| --- | --- |
| `int to;` | 行き先。 |
| `long long weight;` | 正の重み。最短路の cost という命名とは区別。 |
| `int id;` | 論理辺ID。 |

### WarshallFloyd

ソース: [`library/graph/WarshallFloyd.cpp`](../library/graph/WarshallFloyd.cpp)

<!-- catalog-source: library/graph/WarshallFloyd.cpp d788589f3cc2b3b8b48038e57396bc3348611a0a -->

`T` は符号を含むコスト・距離計算に使用する型。`n >= 0`、有向が既定。`INF` は非staticの const メンバで `numeric_limits<T>::max()/4`。辺コストは `-INF < cost < INF`、単純パスの重みの絶対値も INF 未満が前提。
構築直後は初期距離の対角成分が0、その他はINF。`add_edge` ごとに内部辺IDを進めるが **返り値は void**。同じ端点では厳密に小さいコストのみ採用し、同コストの既存辺を保持する。無向時は両向きが同一ID。辺追加で `built=false`、`build()` が初期距離から再計算する。

公開型・名前空間の宣言（テンプレートを含む）:

```cpp
template<class T> struct WarshallFloyd;
```

#### WarshallFloyd

| 完全な公開シグネチャ | 引数・返り値・動作 |
| --- | --- |
| `WarshallFloyd(int n, bool directed = true);` | 頂点数 `n` と有向フラグ `directed` で構築する。結果は未構築。 |
| `void add_edge(int from, int to, T cost);` | `from -> to` にコスト `cost` の辺を登録する。多重辺は最小コストを初期距離へ反映。辺IDは返さない。 |
| `void build();` | 全点対の距離・負閉路影響・復元情報を再計算。O(n^3)。 |
| `T dist(int from, int to) const;` | `from -> to` の距離値。未到達なら INF。`is_negative_infinite(from,to)` が true なら有限最短距離として無効。 |
| `bool reachable(int from, int to) const;` | `from -> to` が到達可能なら true。負閉路影響があっても true。 |
| `bool has_negative_cycle() const;` | グラフのどこかに負閉路があれば true。BellmanFord の始点限定の判定と区別。 |
| `bool is_negative_infinite(int from, int to) const;` | `from` から到達可能な負閉路を経由して `to` に到達できるなら true。 |
| `vector<int> get_path(int from, int to) const;` | `from` から `to` までの頂点列を値で返す。未到達・負の無限大なら空。有効な同一頂点なら `{from}`。 |
| `vector<int> get_path_edges(int from, int to) const;` | 同経路の辺ID列を値で返す。未到達・負の無限大・有効な同一頂点なら空。 |

| 公開変数の宣言 | 意味 |
| --- | --- |
| `int n;` | 頂点数。 |
| `bool directed;` | 有向かどうか。 |
| `bool built;` | 構築結果が有効か。 |
| `int edge_count;` | add_edge の呼出し回数（内部IDの次番号）。 |
| `const T INF = numeric_limits<T>::max() / 4;` | 未到達の番兵値。static 定数ではない。 |
| `vector<vector<T>> initial_distance;` | 直接辺の最小コストと対角0を保持する行列。 |
| `vector<vector<int>> initial_edge;` | 初期距離に対応する辺ID。なければ -1。 |
| `vector<vector<T>> distance;` | 構築した距離行列。 |
| `vector<vector<int>> next_vertex;` | 各点対の経路の次頂点。なければ -1。 |
| `vector<vector<int>> next_edge;` | 各点対の経路の次辺ID。なければ -1。 |
| `vector<vector<bool>> negative_infinite;` | 各点対が負閉路の影響を受けるか。 |
| `bool has_negative_cycle_flag;` | グラフ全体に負閉路があるか。 |

### ModInt

ソース: [`library/math/ModInt.cpp`](../library/math/ModInt.cpp)

<!-- catalog-source: library/math/ModInt.cpp 1e654cdc63d8eccffdf1066bebc13d5c43091db9 -->

`MOD` はコンパイル時の int で `MOD > 1` を static_assert する。逆元・除算は **MOD が素数**かつ除数が0でないことが前提。MOD の素数性を実装が自動検査するわけではない。
`value` は `[0,MOD)` に正規化された値で、直接変更して範囲外にすると演算の前提を壊す。`val()` は値取得の慣習名として維持する。二項演算・比較・入出力は friend 非メンバ関数であり、メンバ関数と区別する。

公開型・名前空間の宣言（テンプレートを含む）:

```cpp
template<int MOD> struct ModInt;
```

#### ModInt

| 完全な公開シグネチャ | 引数・返り値・動作 |
| --- | --- |
| `ModInt(long long x = 0);` | `x` を `[0,MOD)` に正規化して構築する。引数省略時は0。負の x も対応。 |
| `int val() const;` | 正規化された int 値を返す。 |
| `ModInt& operator+=(const ModInt& other);` | 自身に other を加えて自身への参照を返す。 |
| `ModInt& operator-=(const ModInt& other);` | 自身から other を引いて自身への参照を返す。 |
| `ModInt& operator*=(const ModInt& other);` | 自身に other を掛けて自身への参照を返す。 |
| `ModInt& operator/=(const ModInt& other);` | other の逆元を自身に掛け、自身への参照を返す。other は0でなく MOD は素数。 |
| `ModInt operator+() const;` | 自身のコピーを返す。 |
| `ModInt operator-() const;` | 加法逆元を値で返す。 |
| `ModInt pow(long long n) const;` | `n >= 0` の n 乗を値で返す。 |
| `ModInt inv() const;` | MOD が素数で、自身が0でない場合の逆元を値で返す。 |
| `friend ModInt operator+(ModInt lhs, const ModInt& rhs);` | 【friend 非メンバ】lhs + rhs を値で返す。 |
| `friend ModInt operator-(ModInt lhs, const ModInt& rhs);` | 【friend 非メンバ】lhs - rhs を値で返す。 |
| `friend ModInt operator*(ModInt lhs, const ModInt& rhs);` | 【friend 非メンバ】lhs * rhs を値で返す。 |
| `friend ModInt operator/(ModInt lhs, const ModInt& rhs);` | 【friend 非メンバ】lhs / rhs を値で返す。rhs は非零、MOD は素数。 |
| `friend bool operator==(const ModInt& lhs, const ModInt& rhs);` | 【friend 非メンバ】正規化値が同じなら true。 |
| `friend bool operator!=(const ModInt& lhs, const ModInt& rhs);` | 【friend 非メンバ】正規化値が異なるなら true。 |
| `friend istream& operator>>(istream& is, ModInt& x);` | 【friend 非メンバ】is から long long を読み、成功時に x を正規化して更新する。ストリーム参照を返す。 |
| `friend ostream& operator<<(ostream& os, const ModInt& x);` | 【friend 非メンバ】os に x の正規化値を出力し、ストリーム参照を返す。 |

| 公開変数の宣言 | 意味 |
| --- | --- |
| `int value;` | 正規化された剰余値。初期化・演算で `[0,MOD)` を維持する。 |

### Prime

ソース: [`library/math/Prime.cpp`](../library/math/Prime.cpp)

<!-- catalog-source: library/math/Prime.cpp b9dc888e30d593709accc240a459777a363c40f9 -->

同名の struct はなく、公開の通常関数は `is_prime/factorize/divisors/prime_count/primitive_root`。素数列の公開 enumerate 関数は **実装されていない**。入力上限はソースコメント・assert に基づくもので、今回上限付近の速度や数学的正当性を新たに検証したものではない。
prime_count の前計算は最初の必要な呼出しで遅延実行し、内部キャッシュを保持する。`prime_internal` は通常の namespace なのでC++上は外部から参照できるが、内部専用として区別する。

公開型・名前空間の宣言（テンプレートを含む）:

```cpp
namespace prime_internal;
```

#### 通常関数

| 完全な公開シグネチャ | 引数・返り値・動作 |
| --- | --- |
| `bool is_prime(long long n);` | `0 <= n <= 10^18` の素数判定。素数なら true。実装は n<2 を false とするが、上限の assert はない。 |
| `vector<pair<long long, int>> factorize(long long n);` | `1 <= n <= 10^18` を `{素因数, 指数}` の列に分解し、素因数昇順で返す。n=1 なら空。assert は n>=1 のみ。 |
| `vector<long long> divisors(long long n);` | `1 <= n <= 10^18` の正の約数を昇順に列挙して返す。n=1 なら `{1}`。 |
| `long long prime_count(long long n);` | `0 <= n <= 10^11` の n 以下の素数の個数を long long で返す。n<2 なら0。内部前計算・キャッシュを変更し得る。 |
| `long long primitive_root(long long p);` | 素数 `p <= 10^18` の最小の原始根を返す。p=2 なら1。素数性は assert(is_prime(p)) で確認する。 |

#### prime_internal

| 完全な公開シグネチャ | 引数・返り値・動作 |
| --- | --- |
| `long long mod_mul(long long a, long long b, long long mod);` | 【内部】a*b mod mod を __int128 の中間積で計算。正の mod と内部呼出しの非負値が前提。 |
| `long long mod_pow(long long a, long long n, long long mod);` | 【内部】a^n mod mod。内部呼出しでは n>=0、mod>1。 |
| `bool miller_rabin(long long n, long long a);` | 【内部】n に対する基底 a の Miller–Rabin テスト。公開 is_prime が小さい値等を処理してから呼ぶ。 |
| `long long pollard_rho(long long n);` | 【内部】合成数 n の非自明な約数を乱択探索する。公開の一般約数取得ではない。 |
| `void factorize_recursive( long long n, vector<long long>& factors );` | 【内部】n の素因数を重複込みで factors に追加する。n=1 は何もしない。 |
| `void init_sieve();` | 【内部】ふるい・π表・φ表を必要な場合に一度だけ構築する。 |
| `long long phi(long long x, int s);` | 【内部】1..x で最初の s 個の素数に割り切れない個数。前計算済みの素数・表と有効な添字が前提。 |
| `long long isqrt(long long x);` | 【内部】非負 x の整数平方根の床。 |
| `long long icbrt(long long x);` | 【内部】非負 x の整数立方根の床。 |
| `long long iroot4(long long x);` | 【内部】非負 x の整数4乗根の床。 |
| `long long lehmer_pi(long long x);` | 【内部】前計算・キャッシュを使った Lehmer 素数計数。公開 prime_count が準備を行う。 |

| 公開変数の宣言 | 意味 |
| --- | --- |
| `const int sieve_max = 5000000;` | ふるいの固定上限。 |
| `const int phi_n = 10000;` | φ表の x 方向の大きさ。 |
| `const int phi_m = 100;` | φ表の s 方向の大きさ。 |
| `vector<int> sieve_primes;` | ふるいで得た素数列。 |
| `vector<int> pi_table;` | 各値以下の素数個数表。 |
| `long long phi_table[phi_n][phi_m];` | 小さい引数用の φ 表。 |
| `bool sieve_initialized = false;` | 前計算済みか。初期 false。 |
| `unordered_map<long long, long long> lehmer_cache;` | Lehmer の結果キャッシュ。 |

## 4. 既存コードから確認できた共通方針

以下は実装の観察と、今後の設計で優先する基準を区別して読む。正式な要求は [開発規約](LibraryDevelopmentRules.md) に従う。

| 概念 | 既存の名前・共通点 | 新規設計での参照先・判断 |
| --- | --- | --- |
| 型・ファイル | 主な型は PascalCase、定着した略語は LCA/SCC。通常は型名と .cpp 名が一致 | 状態を持たない処理は RLE/Prime/ConvexPolygon の通常関数を参照する |
| 関数・変数 | snake_case。短い慣習名 n/log/prev/val 等、INF/MOD の大文字定数もある | 名前の長さを機械的に揃えず、意味と既存利用法を優先 |
| 追加 | グラフは add_edge、最適化制約は add_profit/add_penalty/add_dependency | add_profit は加算、add_edge は辺の登録。対象を接尾辞で明示 |
| 挿入・削除 | OrderedSet の insert/erase は成功を bool で返す | STLに近い集合操作ではこれを参照。グラフの erase/remove API は現状存在しない |
| 置換・作用 | SegmentTree/LazySegmentTree の set は代入、apply は作用 | 加算・置換・写像を混同しない。add を単なる代入の名前に使わない |
| 取得 | セグ木 get、フロー get_edge、最短路 dist、ModInt val | 全てに get_ を付ける規則はない。対象分野の既存APIを優先 |
| 判定 | same/contains/empty/reachable、has_cycle/has_negative_cycle、is_prime/is_convex | 同一性・包含・有無・状態を区別する。戻り値は bool |
| 範囲操作 | prod(left,right)、apply(left,right,f) は [left,right) | SegmentTree と LazySegmentTree を対で参照。左右端・作用の順序を維持 |
| 境界探索 | max_right/min_left は集約値に対する判定関数。lower_bound/upper_bound は値に対する比較順 | 操作が異なるため同じ名前へ統合しない |
| 復元 | get_path は頂点列、get_path_edges は辺ID列。get_cycle_vertices/get_cycle_edges、diameter_vertices/diameter_edges | 複数の要素種がある場合は対応関係・列長・向きを明記 |
| グラフ | from/to または u/v、始点終点は start/source/sink、元の辺IDは追加順の0始まり | 最短路は Dijkstra/BellmanFord、ネットワークは MaxFlow/MinCostFlow、木は LCA/TreeDiameter |
| 個数・サイズ | n は頂点・論理要素数、edge_count/group_count、count()/size() | size メンバは内部葉数の場合もある。数える対象を必ず明記 |
| 重み・距離・費用 | 最短路は cost と dist/distance 配列、木の辺は weight、フローは capacity/flow/cost | 同じ cost でも単位費用と総費用を区別する |
| 初期化・構築 | 必要な n/設定/初期配列をコンストラクタに渡す。グラフ結果は build、最適化は solve/flow | コンストラクタは辺登録可能な状態を作る。結果取得に build が要る型は呼出し順を明記 |
| 構築の有効性 | 多くのグラフは built と辺追加時の無効化。CycleDetection は searched の遅延探索 | LCAには built がない。全ライブラリが自動再構築するとは考えない |
| 型 | 添字・頂点・個数は主に int。容量・利益・重みは long long。最短路/セグ木は T | 外部の ll エイリアス・rep 等に依存しない。cross/中間積に __int128 を使う例あり |
| コメント | `// 操作: name(引数の意味) -> 返り値・対象区間`、必要な前提の追記 | UnionFind が代表。演算子は `// 加算` 等の慣習的な短い見出しもある |
| 書式 | `<bits/stdc++.h>`、`using namespace std;`、4スペース、struct中心、関数間の空行 | 関連実装を優先して合わせる。初期化リストの折返し・i++/++i は混在し、単一ルールとは認定しない |

### 状態変更と参照の区別

| 操作群 | 論理状態・内部状態への影響 |
| --- | --- |
| insert/erase/set/apply/merge/add_* | 保持内容・制約・グラフ等を変更する |
| build/solve/flow/slope | 計算結果・残余容量などを変更する。flow は単なる照会ではない |
| UnionFind の leader/same/size/groups | 論理的な照会でも経路圧縮で内部の親配列を変更し得る |
| LazySegmentTree の get/prod/max_right/min_left | 論理的な照会でも遅延伝播で内部配列を変更し得る |
| CycleDetection の判定・復元 | 必要なら探索してキャッシュを構築する |
| 多くのグラフの const 取得、OrderedSet の const 検索、ModInt の val/pow/inv | 論理内容を変更せず参照・値計算する |
| Prime の素因数探索・素数計数 | 外部からは値計算だが、内部乱数生成器・前計算・キャッシュを変更し得る |

`const` の有無だけで「利用者から見た更新か照会か」を決めない。新規設計では上記の内部変更を考慮し、適切な const を付けるが、既存宣言をこのためだけに変更しない。

## 5. 相違点・例外と維持する理由

| 論点 | 実装上の相違 | 判断・扱い |
| --- | --- | --- |
| add_edge の返り値 | Dijkstra/BellmanFord/CycleDetection/MaxFlow/MinCostFlow は int、SCC/LCA/TreeDiameter/WarshallFloyd と内部 FlowGraph は void | 同名でも返り値は同一ではない。辺IDを外へ返す必要と既存の利用法が異なる。今回は変更しない |
| 有向フラグ | Dijkstra/BellmanFord/WarshallFloyd は既定 true、CycleDetection は必須、木は常に無向、SCC/フローは常に有向 | フラグの省略可否を型ごとに確認する。木に汎用フラグを追加しない |
| 距離取得の名前 | 最短路は dist、LCA は distance、木の直径は diameter_length | 対象と意味が異なる。既存呼出しを壊して事後的に揃えない |
| サイズ・個数 | UnionFind.size(v) は成分サイズ、OrderedSet.size() は全体サイズ、UnionFind/SCC.count() は成分数、セグ木の size は内部の葉数 | 引数・所属型・数える対象まで一緒に確認する |
| groups の戻り方 | UnionFind はコピー、SCC は const 参照 | 前者は要求時に集計し、後者は構築済み結果を保持する。生成・保持方法に沿った違い |
| 頂点列・辺列の戻り方 | 最短路とサイクルはコピー、直径は const 参照 | コピーの独立性と参照の有効期間を区別。参照を保持して再構築する際は内容変更に注意 |
| 復元の名前 | get_path 系、get_cycle_* 系、diameter_* 系 | 1つの確定済み直径の取得と、終点指定の経路復元は用途が違う。get_ の一律追加は不要 |
| 復元の列長 | パスは頂点数=辺数+1、サイクルは始点重複なしで頂点数=辺数 | 閉路の閉じる辺の扱いを通常パスへ合わせない |
| 負閉路の判定範囲 | BellmanFord.has_negative_cycle は始点から到達可能な部分、WarshallFloyd はグラフ全体 | 同名でも判定対象が異なる。BF.is_negative(v) と WF.is_negative_infinite(from,to) の対象の違いも維持 |
| 未到達の表現 | Dijkstra は INF、WarshallFloyd は INF、BellmanFord は reached を別に持つ | BF.dist の無効値を INF と推測しない。各APIの reachable・負閉路判定を使う |
| INF | Dijkstra は static constexpr max()/2、WarshallFloyd は非static const max()/4、MinCostFlow の max()/4 は作業距離の初期値（到達性は別管理） | 値・宣言・距離の扱える範囲まで異なるため一律の共通定数にはしない |
| cost/weight | 最短路は cost、木の直径は weight、MCF.EdgeInfo.cost は単位費用、FlowResult.cost は追加総費用 | 同じ数値型でも意味を明記する。型・所属情報なしに cost を比較しない |
| capacity | フローの Edge は残余容量、EdgeInfo は元の容量 | 同名でも保存対象が違う。get_edge が元容量を復元することを明記 |
| edges | BellmanFord.edges は公開配列（無向で2件）、MaxFlow/MinCostFlow.edges() は元の辺の情報を返す関数 | 同名を見て同じ型・件数・用途と判断しない |
| 復元用変数 | parent_vertex/parent_edge、prev/prev_edge、previous_vertex/previous_edge、parent 等が混在 | ローカル・内部の表現を事後的に統一しない。新規実装では近い処理の既存ソースを優先 |
| 命名上の例外 | ModInt.val、OrderedSet.iterator、prime_internal、INF/MOD | 数値型・STL互換の慣習、内部名前空間、定数の役割に沿う。PascalCase/snake_case の機械的な置換はしない |
| public の内部補助 | LazySegmentTree.update/all_apply/push、ProjectSelection.FlowGraph、各内部配列、prime_internal | 規約の軽量な公開設計による。通常の利用APIとして新規設計へ丸ごと追加しない |
| 計算の実行回数 | グラフ build は再計算可、CycleDetection は遅延キャッシュ、ProjectSelection.solve は1回のみ、MCF は追加禁止状態を持つ | 名前だけで再実行・変更可否を判断しない |

この表は違いの発見を記録するもので、全てを統一すべきという提案ではない。命名変更、引数順序・型・既定値・返り値・参照・動作の変更を行う場合は、開発規約の破壊的変更手順を適用する。

## 6. 新規設計前の確認手順

1. [開発規約](LibraryDevelopmentRules.md) を読み、このカタログの確認SHAと現在のGitHubソースを照合する。参照できない場合は未確認のファイルとコミットを明記し、必要なソースだけ提示してもらう。記憶からAPIを補完しない。
2. 最新の [UnionFind.cpp](../library/data_structure/UnionFind.cpp) を読み、型・コメント・添字・初期化・全体の書式を確認する。常設コピーがあってもGitHub版で確認する。
3. 以下の表から近いAPIを選び、**関連する実装本体**を読む。名前だけの類似より、操作の対象・意味・保存方式が近いものを優先する。
4. 新規API案に完全なシグネチャ、引数の意味・順序、返り値、更新か照会か、前提、未該当値、構築順、コピー／参照と、ソース・テストの予定パスと分類理由を記す。配置先はRepositoryLayoutに従ってAIが決める。
5. 同じ意味の別名・同じ名前の別の意味を横断検索する。違いを残す場合は理由を説明する。通常の新規開発・ソース修正では要件と既存設計からAPI案を示し、明示的な承認後に実装へ進む。破壊的変更や決められない重要な仕様は、開発規約・AGENTS.mdに従って確認する。

| 新規設計の対象 | まず読む既存ソース |
| --- | --- |
| 成分・代表元・成分数 | UnionFind、SCC |
| 点更新・区間集約・境界探索 | SegmentTree、LazySegmentTree |
| 遅延作用の合成・作用順 | LazySegmentTree（mapping/composition/id と呼出し順） |
| 順位・集合・イテレータ | OrderedSet |
| 重み付き経路・復元・負閉路 | Dijkstra、BellmanFord、WarshallFloyd |
| 木の祖先・距離・構成要素の列 | LCA、TreeDiameter |
| 閉路の頂点列・辺ID列 | CycleDetection |
| 容量・流量・残余辺・結果型 | MaxFlow、MinCostFlow |
| 利益・制約追加・一度だけの最適化 | ProjectSelection |
| 値型・演算子・剰余 | ModInt |
| 状態を持たない通常関数 | RunLengthEncoding、ConvexPolygon、Prime |

## 7. 追記・更新用の形式

新しいライブラリは全対象一覧に1行を追加し、第3節に既存と同じ形式で次を追記する。共通点・例外が増えた場合だけ第4〜6節も更新する。手順の詳細は [LibraryWorkflow.md](LibraryWorkflow.md)。

```text
### ライブラリ名
ソースへの相対リンク
catalog-source コメント: library/カテゴリ/ファイル.cpp と、git hash-object で取得した40桁のblob SHA
確認したコミット: 完全なコミットSHA（部分更新時はその項目に記す）
テンプレート・前提・初期状態・構築順・添字／区間
公開型・名前空間の宣言
完全な公開シグネチャと引数・返り値・動作の表
公開変数の宣言と意味の表
通常利用APIとpublicの内部補助を区別した説明
既存との相違・合理的な例外・未確認事項
```

`python3 tools/check_catalog.py` で対象漏れ・ソース変更・ローカルリンクを検出する。この確認はAPIの意味やアルゴリズムの正しさ、テストの通過を判定しない。SHAだけを更新して確認を済ませず、宣言・関数本体・前提条件を読む。

## 8. 未確認事項と調査範囲

- 対象リポジトリ、main、当時の全18 `.cpp` と README、ソース内の公開宣言・実際の処理を確認済み。別リポジトリ・ローカル専用・未コミットのライブラリは対象外で、存在も未確認。
- 過去チャットのAC報告を現行ソース版の新たな検証結果として扱わない。現在はオンラインAC・再ACを開発の必須工程に含めない。
- 2026-10-12の再整備では全18ソースを読み、MinCostFlowの距離と未到達の衝突を修正した。公開シグネチャは維持。全18の保存済み回帰テストを追加して検証するが、全入力・最大規模・全テンプレート型の正当性を認定しない。具体的な検証結果と範囲は [DevelopmentLog.md](../records/DevelopmentLog.md) と [tests/README.md](../tests/README.md) を参照する。
- C++23 の運用環境でも `<bits/stdc++.h>`・`__int128` はGNU系の拡張であり、全C++コンパイラでの移植性を認定していない。
- テンプレート引数が満たす代数的条件や距離・費用の範囲は利用側でも確認する。assert が全条件を検証するとは限らず、無効化した assert によって前提が消えるわけではない。
