# RepositoryLayout — フォルダ構成と自動配置

## 1. 配置はAIが行う

新規ライブラリの配置先は、AIが用途・前提条件・既存構成から決める。ユーザーは作りたい機能を伝えるだけでよく、フォルダの指定・作成・ファイル移動・コマンド入力は不要。

設計案にはソースとテストの予定パス、分類理由を併記する。通常の配置判断について独立した承認を求めず、API案の承認に含める。承認後、AIが必要なフォルダを作成し、ソース・テスト・目次・カタログ・スニペット・記録を同期してGitHubへ反映する。

## 2. ライブラリ専用フォルダとカテゴリ

ライブラリ本体はすべて `library/` にまとめ、その下のカテゴリへ配置する。`Rule/` には規約・手順・カタログを置き、ルート直下にはカテゴリを作らない。カテゴリ別の目次は [library/README.md](../library/README.md)、リポジトリ全体の入口は [README.md](../README.md) とする。

| フォルダ | 主な対象 | 現在の例 |
| --- | --- | --- |
| `library/data_structure/` | 集合、配列、区間集約などの汎用データ構造 | UnionFind、OrderedSet、SegmentTree、LazySegmentTree |
| `library/graph/` | 一般のグラフ、最短路、閉路、連結成分、ネットワークフローとその応用 | Dijkstra、BellmanFord、WarshallFloyd、CycleDetection、SCC、MaxFlow、MinCostFlow、ProjectSelection |
| `library/tree/` | 入力や構造を木・森に限定するグラフアルゴリズム | LCA、TreeDiameter |
| `library/math/` | 数論、剰余演算など | Prime、ModInt |
| `library/geometry/` | 点、図形、幾何計算 | ConvexPolygon |
| `library/algorithm/` | 上記に当てはまらない処理 | RunLengthEncoding |

名前にTreeが含まれることだけで `library/tree/` に分類しない。SegmentTree・LazySegmentTree・今後のFenwickTreeは `library/data_structure/`。最小全域木のように一般のグラフを入力として扱うものは `library/graph/`。木を前提とする祖先・直径・木DP・Heavy-Light Decomposition等は `library/tree/` とする。

複数のカテゴリに関連する場合は、主な入力・用途を基準に既存の最も近いカテゴリを選ぶ。同じソースを複数フォルダへ複製しない。現在のカテゴリで整理できない新分野では、AIが内容を表す小文字の `snake_case` のカテゴリを `library/` 以下へ新設してよい。分類の都合だけで公開APIを変更しない。

## 3. ソースとテストの対応

- 本体は原則 `library/カテゴリ/LibraryName.cpp`。1ライブラリ1ファイルとし、テスト用 `main()` を含めない。
- 対応する回帰テストは `tests/カテゴリ/LibraryName.test.cpp`。`library/` 以下と同じ相対構成にし、`#include "library/カテゴリ/LibraryName.cpp"` で対象を読む。例: `library/tree/LCA.cpp` と `tests/tree/LCA.test.cpp`。テストを `library/` へ混在させない。
- 関連するテスト補助ファイルは必要なテストの近くに置く。補助ファイルは `.test.cpp` と区別し、単独実行テストとして検出されない名前にする。
- ライブラリのファイル名はリポジトリ全体で重複させない。スニペット名・prefixはカテゴリによらず `LibraryName`・`!LibraryName`。

スニペット生成と共通チェックは、`library/` 以下の `.cpp` をカテゴリ名の固定一覧なしで自動検出する。`library/tree/` や新しいカテゴリを追加しても、収録・構文確認の対象になる。共通チェックでは本体とテストのパス・対象includeの対応と、`library/` 外やカテゴリ外に残った本体も確認する。

## 4. リポジトリ内の役割

| 場所 | 内容 |
| --- | --- |
| `library/` | カテゴリ別のライブラリ本体 |
| `library/README.md` | ライブラリのカテゴリ別目次 |
| `Rule/` | 規約、運用手順、命名・APIカタログ、Projectの固定指示 |
| `templates/AtCoder.cpp` | 指定のAtCoderテンプレート |
| `snippets/competitive_programming.code-snippets` | !atcoderと全ライブラリの生成スニペット |
| `tests/` | 回帰テストとテスト補助 |
| `tools/` | 生成・整合性確認・検証のスクリプト |
| `records/DevelopmentLog.md` | 変更理由、対象版、検証結果・範囲・未確認事項 |
| `AGENTS.md` | AIの入口 |
| `README.md` | リポジトリ全体と管理資料への入口 |

管理用フォルダにライブラリ本体を置かない。

## 5. 既存パスを移動する場合

既存ファイルの移動は、対象と移行先を明らかにし、ユーザーが了承した範囲で行う。今回の全18本体の `library/` への集約は明示的な移行依頼である。ソースの移動、必要なテストの移動や対象includeの更新、README、APIカタログのリンクと `catalog-source` のパス、関連する規約・チェック・記録を同じコミットで更新する。

移動だけの場合も共通チェックを実行し、ソースのGit blob SHAが移動前と一致すること、スニペットの展開本文・名前・prefixが維持されることを確認する。スニペットの掲載順は新しいパス順で再生成する。古いパスの互換コピーは作らず、利用側のincludeやブックマークは新しいパスへ変更する。Gitの履歴は削除しない。

2026-10-12（日本時間）の移動:

| 移動前 | 移動後 |
| --- | --- |
| `algorithm/`（1本） | [library/algorithm/](../library/algorithm/) |
| `data_structure/`（4本） | [library/data_structure/](../library/data_structure/) |
| `geometry/`（1本） | [library/geometry/](../library/geometry/) |
| `graph/`（8本） | [library/graph/](../library/graph/) |
| `math/`（2本） | [library/math/](../library/math/) |
| `tree/`（2本） | [library/tree/](../library/tree/) |

先行したgraph/treeの分離を維持する。今回はテストのカテゴリとファイル位置を維持し、全18テストの対象includeへ `library/` を付ける。履歴の過去パスは記録として残し、現行のリンクは移動先へ合わせる。

日常の依頼・承認・反映は [LibraryWorkflow.md](LibraryWorkflow.md)、Workとチャットの使い方は [CompetitiveProgrammingGuide.md](CompetitiveProgrammingGuide.md) に従う。

改訂日: 2026-10-12（日本時間）。
