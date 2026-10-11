# 開発作業の記録

正式版はGitHubのmain。本書に変更理由・対象ソース版・実施した検証と範囲・結果・未確認事項を残す。GitHub Actionsの結果は該当コミットの実行履歴で確認する。現在の完了条件は [開発規約](../Rule/LibraryDevelopmentRules.md) 第13節に従う。

過去の項目は当時の方針と検証結果の履歴であり、オンラインACや再AC待ちに関する以前の条件は、下記の運用変更で廃止した。

## 2026-10-12（日本時間） — Ruleへ再構成し、設計承認後の自動開発を整備

基準main: [34eede7d6d376275e1f25bb93403e63b8abd9bc8](https://github.com/primesosu91/algorithm/commit/34eede7d6d376275e1f25bb93403e63b8abd9bc8)。

### 許可された作業

ユーザーは既存docsの削除、Rule等への規約再構成、既存18ソースの必要な修正、現在のスニペット生成、コミットとGitHub反映を依頼した。今後は公開API案を提示して承認を受け、以後の実装・検証・生成・反映を自動で完了する。

### 確認と変更

- 全18ソースを全文確認し、取得版のGit blob SHAが基準mainと一致することを確認。
- 旧docsとLibraryの3規約は終端改行を除き同内容。従来の実装・コメント・型・添字・区間・デバッグ・検証・正式採用の要件を引き継いだ。
- docsを廃止し、Ruleに規約・APIカタログ・手順・Project指示・スニペット仕様を再構成。履歴はGitに維持する。
- AGENTSで設計案の明示的承認を必須化し、承認後の通常作業とGitHub反映には繰り返しの承認を求めない方針を設定。
- 不具合修正の説明と完全なソースを提供する要件は維持。GitHubのライブラリ更新では全文ソースへのリンクでも提供できるようDebugRulesに対象限定の補足を追加。
- ユーザー指定AtCoderテンプレートを保存。先頭!atcoder、solve最初の行の$0、全18全文スニペット、prefix/bodyだけの形式を生成。
- 原本・スニペット・カタログの同期、各ソースとテンプレートの組合せのC++23構文確認、18ライブラリの独立した回帰テスト、ASan/UBSan確認を用意。
- MinCostFlowのみ必要な動作修正。公開関数名・引数・返り値・型は変更していない。他17ソースは取得版と同じ内容。
- Projectの指示はユーザーが一度貼り替える。手元のVS Codeへのスニペット取り込みは、GitHubの生成更新とは別の操作として案内する。

### MinCostFlowの原因と反例

2頂点のグラフで、0→1に容量1・単位費用2305843009213693951の辺を追加し、flow(0,1,1)を呼ぶ。期待は流量1・費用2305843009213693951、旧実装は流量0・費用0だった。流量・費用はlong long内に収まる。

旧実装は内部距離をmax()/4で初期化し、その値以上の経路を未到達として捨てていた。初期ポテンシャルと最短増加路の両方で到達性を別フラグに分け、距離の初期値だけで未到達と判定しないよう修正した。中間計算は引き続き__int128と既存のassertで確認する。

修正ソースはblob `eb45f905e889965b10c6730c150ecd3635816d7a`。必要な再テスト後も実際の再ACまでは検証中。

### 検証結果

- g++ 13.3.0、C++23、-Wall/-Wextraで全18ソースの構文確認に成功。
- 指定テンプレート単体と、各18ライブラリを挿入したコードの構文確認に成功。全18を同時に挿入したコードも構文確認に成功。
- 全18の回帰テストが通常実行で成功。最短経路と負閉路の単純パス／サイクル全列挙、全カット、全流量、全選択部分集合、通常配列、std::set、親の逐次移動、独立したふるい等と比較。
- 全18の回帰テストがAddressSanitizer/UndefinedBehaviorSanitizer実行で成功。ローカル実行環境でLeakSanitizerは/procの読み取り制限により起動できなかったため、この実行のみASAN_OPTIONS=detect_leaks=0とした。配列外参照・解放後使用・未定義動作等の検査は有効。GitHub Actionsの標準のサニタイザ実行ではリーク検査を無効化しない。
- スニペットの独立した展開スキャナによる4検証に成功。全19項目、先頭!atcoder、solve最初の行の4スペース直後の$0、ライブラリ全文一致、文字としてのドル・バックスラッシュ、誤った冒頭行・重複名の検出を確認。
- カタログ全18項目のblob SHA・収録・相対リンク整合に成功。
- GitHub Actionsの初回結果は、mainへ反映した後に別途確認する。ローカルの成功をGitHubで実行済みと扱わない。

### 当時の残る確認

- 全入力・最大規模・全テンプレート型の正当性や性能を認定するものではない。
- 過去の最初のAC問題・提出情報と各版の対応は未移行。以前の採用状態は今回の移行だけで取り消さない。
- MinCostFlowの修正版は再AC待ち。オンラインジャッジへのログイン・提出を実行済みとして扱わない。

## 2026-10-12（日本時間） — オンラインACと採用状態の個別管理を廃止

基準main: [b56c21d1343fc01982ba8a41ca0c0a60e02f217b](https://github.com/primesosu91/algorithm/commit/b56c21d1343fc01982ba8a41ca0c0a60e02f217b)。

### 許可された作業と変更

- ユーザーはオンラインAC確認を省く運用への変更、不要となるGitHubファイルの削除、関連資料の修正を依頼し、`records/LibraryStatus.md` の削除を了承した。
- 同ファイルを削除し、AGENTS・README・Rule・テスト説明・自動チェックの参照と必須ファイル指定を修正。採用状態の代替ファイルは作らない。
- オンライン提出・AC・再ACと「検証中／正式採用」の個別管理を必須工程から外し、C++23の対象固有の検証、整合性確認、資料同期、コミット・GitHub反映を完了条件とした。
- 動作変更後の回帰テストと再検証、検証結果・範囲・未確認事項の記録、GitHub Actionsの確認は継続する。ローカル検証とActionsの結果をオンラインACとして報告しない。
- MinCostFlowの再AC待ちは現在の完了条件から外した。ソースはblob `eb45f905e889965b10c6730c150ecd3635816d7a` のままで、以前のテスト結果は上記の履歴を参照できる。
- 18ライブラリ、AtCoderテンプレート、19項目のスニペット、保存済み回帰テストとGitHub Actionsの実行内容は変更していない。
- Projectの指示はGitHubから直接変更できないため、更新した [ProjectInstructions.md](../Rule/ProjectInstructions.md) へユーザーが貼り替える。

### 検証と未確認事項

- 基準mainの全58ファイルをGit blob SHAで照合し、同一内容の作業版から変更した。
- `python3 tools/check_repository.py` が成功。g++ 13.3.0・C++23で全18ソース、テンプレート単体、各ライブラリとの組合せ、全18を同時に挿入したコードの構文確認と、全18回帰テストの通常実行を通過した。
- 全19スニペットと原本の整合性、展開文字列・カーソル位置等の4テスト、カタログの全18ソース版と相対リンクを確認した。スニペットを再生成しても内容は変更されなかった。
- 廃止ファイルへの現行参照・必須ファイル指定・ACを必須とする手順が残っていないこと、削除が1ファイルだけであること、対象外の全ファイルが基準版と同じ内容であることを確認した。
- 今回はライブラリの動作を変更していないため、ローカルでのASan/UBSan再実行は省略。当該コミットのGitHub Actionsは通常検証とASan/UBSanを反映後に実行する。その結果はローカル実行結果と区別して完了報告する。

## 2026-10-12（日本時間） — graph/treeを分離し、自動配置とWork運用を整備

基準main: [416edcb1754776c006600bda50a7bcb46a5588a6](https://github.com/primesosu91/algorithm/commit/416edcb1754776c006600bda50a7bcb46a5588a6)。

### 許可された作業と変更

- ユーザーは新規ライブラリのフォルダ選択・作成・配置もAIが自動で行うこと、グラフと木のフォルダ分離、関連資料の修正、Workでの新規チャットを基本とする運用への変更とGitHub反映を依頼した。
- 基準mainの全57ファイルをGit blob SHAで照合し、同一内容の作業版から変更した。AGENTS・該当規約・命名/API共通方針と対象項目・UnionFind・対象の本体とテスト・関連記録・共通チェックを確認した。
- [RepositoryLayout.md](../Rule/RepositoryLayout.md) を新設。主な入力・用途からAIが配置先を決定し、設計案にソース・テストの予定パスと分類理由を併記する。必要なフォルダ作成はAIが担当し、通常の配置について別の承認やユーザーの操作を求めない。
- 木を前提とするLCAとTreeDiameterの本体を `graph/` から `tree/`、対応テストを `tests/graph/` から `tests/tree/` に移動。テストの対象includeを新しいパスへ合わせた。一般のグラフ8本はgraph、SegmentTree等はdata_structureに置く。
- README・APIカタログの分類とリンク、catalog-sourceのパス、AGENTS・開発規約・運用ガイド・手順・Project指示・スニペット規約・テスト説明を同期した。
- 新規作成・修正・規約改訂・環境整備は原則新しいWorkチャットで開始する。関連する以前のチャットが残っていれば、Workでその履歴を継続できる。新規チャットでも最新mainの規約・実装・テスト・記録から開始し、過去会話や常設添付を必須にしない。GitHubの変更作業はWorkで実行する。
- 共通チェックに、本体と同じ相対構成の回帰テスト、その対象include、取り残されたテストの確認を追加。treeや今後の新カテゴリも固定一覧なしで自動検出する。新しい配置規約を必須資料へ追加した。
- 配置チェック用の4回帰テストを追加。スニペットは新しいパス順で再生成し、!atcoderを先頭に保った。

### 対象ソース版と移行

| 移動前 | 移動後 | Git blob SHA |
| --- | --- | --- |
| `graph/LCA.cpp` | [tree/LCA.cpp](../library/tree/LCA.cpp) | `9cc01adcd28abd1c975ca854751cb0bce9b80cc1` |
| `graph/TreeDiameter.cpp` | [tree/TreeDiameter.cpp](../library/tree/TreeDiameter.cpp) | `af703fa4be1ee9dc465e4b63bf9e15f076a578a5` |

両本体を含む全18ソースの内容・公開API・blob SHAは基準mainと同じ。テスト2本はincludeのパスだけ変更した。古いパスを使う利用側のincludeやブックマークは新パスへ変更する。ソースの重複コピーは作らず、Gitの履歴を維持する。

### ローカルの検証結果と範囲

- `python3 tools/generate_snippets.py`、`python3 tools/check_repository.py` が成功。g++ 13.3.0・C++23で全18ソース、テンプレート単体、各ライブラリとの組合せ、全18同時の構文確認、全18回帰テストの通常実行を通過した。
- 配置チェックの4テストが成功。tree・新カテゴリの自動検出、本体だけを移した場合のテスト不足と旧テスト、両方を移した後の古いinclude、カテゴリ外配置の検出を確認した。既存スニペットの4テストと合わせてPythonの8テストが成功した。
- 全19スニペットの各項目を基準版と比較し、名前・prefix・bodyがすべて同一で掲載順のみの変更であることを確認。solveの$0、空行・コメント・インデント・エスケープの展開一致も確認した。
- カタログの全18ソース版・収録、ソースとテストの対応、管理資料の相対リンク、差分の空白を確認した。移動前のパスは移行表にのみ記載し、現行のリンク・include・catalog-sourceに残っていない。
- ライブラリの動作は変更していないため、ローカルでのASan/UBSan再実行は省略。GitHub Actionsは反映後に通常検証とASan/UBSanを実行する。その結果は当該コミットの実行履歴で確認し、ローカルの成功とは区別して完了報告する。

### 残る設定と未確認事項

- Projectの設定欄はGitHub更新だけでは変わらないため、ユーザーが更新した [ProjectInstructions.md](../Rule/ProjectInstructions.md) の全文へ貼り替える。
- GitHubの生成スニペット更新と手元のVS Codeへの取り込みは別の操作。掲載順以外の内容は同じであり、既存のprefixはそのまま使える。
- 保存済みテストの範囲を越えた全入力・最大規模・全テンプレート型の正当性や性能を新たに認定する変更ではない。

## 2026-10-12（日本時間） — ライブラリ本体をlibrary/へ集約

基準main: [ca75829056fbc9c6c6d927645dec8a1448fa7412](https://github.com/primesosu91/algorithm/commit/ca75829056fbc9c6c6d927645dec8a1448fa7412)。

### 許可された作業と変更

- ユーザーは規約とライブラリ本体を区別するため、ライブラリ専用フォルダの作成と既存本体の移動を依頼し、旧開発チャットを削除してよいか確認した。
- 基準mainの全59ファイルをGit blob SHAで照合し、同一内容の作業版から変更した。AGENTS、開発・運用・配置・命名/API・スニペット規約、UnionFind、対象ソースとテストのinclude、生成・検証処理、既存作業記録を確認した。
- 18本体を `library/` 以下の6カテゴリへ移動し、graph/treeの分離を維持した。ソース内容・公開APIは変更していない。旧カテゴリへの互換コピーは作らず、Gitの履歴を維持する。
- `library/README.md` をカテゴリ別目次として追加し、ルートREADMEをリポジトリ全体と管理資料への入口にした。AGENTS・規約・手順・Project指示・カタログ・現行リンクを新パスに合わせた。過去の作業記録のパス表記は当時の履歴として残し、リンク先は現在の本体に合わせた。
- 対応テストは `tests/カテゴリ/LibraryName.test.cpp` のままで、本体の `library/` 以下と相対構成を揃えた。全18テストは対象includeにlibrary/を付ける変更だけで、独立した比較方法・入力範囲・seedは維持した。
- スニペット生成・カタログ・構文チェックはlibrary/以下の全カテゴリからソースを検出する。共通配置チェックはlibrary/外やカテゴリ外の本体、対応テストの不足・取り残し、古いincludeを検出する。library/READMEの存在と相対リンクも確認する。
- 完了済み開発チャットの保持を必須としないことを運用ガイド・手順・AGENTS・Project指示へ追記した。削除前にGitHubへの反映を確認し、チャットだけに残る未完了の要望・設計・承認・差分はGitHubの記録や作業ブランチへ保存する。過去会話の全文移行・完全復元を保証する変更ではなく、チャットの削除操作はユーザーが行う。

### 対象ソース版

全18本体のGit blob SHAは基準mainと同じ。移動前のパスは下表から先頭の `library/` を除いたもの。

| 移動後のソース | Git blob SHA（移動前と同じ） |
| --- | --- |
| [library/algorithm/RunLengthEncoding.cpp](../library/algorithm/RunLengthEncoding.cpp) | `2490c1cdd542e3b0eee31fc02180f7e0e72cfae2` |
| [library/data_structure/LazySegmentTree.cpp](../library/data_structure/LazySegmentTree.cpp) | `73ca088f5d8446329dd2a85ab82a41c15e94fa3b` |
| [library/data_structure/OrderedSet.cpp](../library/data_structure/OrderedSet.cpp) | `d5e69129b4e8361fc496f7c469fa2bf6fda4bf6b` |
| [library/data_structure/SegmentTree.cpp](../library/data_structure/SegmentTree.cpp) | `03ff41bbb1f74eb970106545266be594a043dc13` |
| [library/data_structure/UnionFind.cpp](../library/data_structure/UnionFind.cpp) | `52213a1f40c4239411e43aacb768b17e299ee9f0` |
| [library/geometry/ConvexPolygon.cpp](../library/geometry/ConvexPolygon.cpp) | `befd23138399b2f46a87732bf52ecea24e6537e3` |
| [library/graph/BellmanFord.cpp](../library/graph/BellmanFord.cpp) | `604849c233489ba68cfd05b4161d08b9fc911b58` |
| [library/graph/CycleDetection.cpp](../library/graph/CycleDetection.cpp) | `c5f5544dc440c301083a3e1a402b0fe69289e48b` |
| [library/graph/Dijkstra.cpp](../library/graph/Dijkstra.cpp) | `8f95b4587063347fdf154fcb05f19f19aa8550e5` |
| [library/graph/MaxFlow.cpp](../library/graph/MaxFlow.cpp) | `0efd635e006549d72006e2f61078699e160dc8e5` |
| [library/graph/MinCostFlow.cpp](../library/graph/MinCostFlow.cpp) | `eb45f905e889965b10c6730c150ecd3635816d7a` |
| [library/graph/ProjectSelection.cpp](../library/graph/ProjectSelection.cpp) | `087135952c0e40cdcfcd243c196256488ebf282d` |
| [library/graph/SCC.cpp](../library/graph/SCC.cpp) | `a9cf9244049a055edcbaa287c7d3c8d0573cabb2` |
| [library/graph/WarshallFloyd.cpp](../library/graph/WarshallFloyd.cpp) | `d788589f3cc2b3b8b48038e57396bc3348611a0a` |
| [library/math/ModInt.cpp](../library/math/ModInt.cpp) | `1e654cdc63d8eccffdf1066bebc13d5c43091db9` |
| [library/math/Prime.cpp](../library/math/Prime.cpp) | `b9dc888e30d593709accc240a459777a363c40f9` |
| [library/tree/LCA.cpp](../library/tree/LCA.cpp) | `9cc01adcd28abd1c975ca854751cb0bce9b80cc1` |
| [library/tree/TreeDiameter.cpp](../library/tree/TreeDiameter.cpp) | `af703fa4be1ee9dc465e4b63bf9e15f076a578a5` |

### ローカルの検証結果と範囲

- `python3 tools/generate_snippets.py` と `python3 tools/check_repository.py` が成功。g++ 13.3.0・C++23で全18本体、指定テンプレート単体、各ライブラリとの組合せ、全18を同時に挿入したコードの構文確認と、保存済み全18回帰テストの通常実行を通過した。
- 配置の5テストとスニペットの4テスト、計9件が成功。library/内のtree・新カテゴリの自動検出、移動時のテスト不足・旧テスト・古いinclude、ルート・管理フォルダ・旧カテゴリ・library直下への本体配置を確認した。全19項目の原本との全文一致・キー・prefix・solveのカーソル位置・エスケープを確認した。
- 全18ソースのblob SHA、カタログの収録と版、目次の全18リンク、本体とテストの対応、管理資料の相対リンク、差分の空白を確認した。削除は移動元18ソースのみ、追加は移動先18ソースとlibrary/READMEのみである。
- 再生成した19項目のスニペットとAtCoderテンプレートは基準mainとバイト単位で同一。手元のVS Codeの既存スニペットはそのまま使える。
- 本体の動作は変更していないため、ローカルでのASan/UBSan再実行は省略。GitHub Actionsは反映後に通常検証とASan/UBSanを実行する。当該コミットの結果は実行履歴で確認し、ローカル検証とは区別して完了報告する。

### 残る設定と未確認事項

- Projectの指示はGitHubへの保存だけでは変わらないため、ユーザーが [ProjectInstructions.md](../Rule/ProjectInstructions.md) の全文へ貼り替える。本体をincludeしている利用側のコードやブックマークはlibrary/を付けた新パスへ更新する。
- 既存18ライブラリと現行資料はGitHubから参照でき、次回の新しいWorkに旧チャットや常設添付は不要。チャットだけの未保存情報・途中の作業があるかは、削除前にその対象を確認する。すべての過去チャットの全文を保存したとは扱わない。
- 保存済みテストの範囲を越えた全入力・最大規模・全テンプレート型の正当性や性能を新たに認定する変更ではない。オンライン提出・AC・再ACは実行していない。
