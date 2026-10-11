# 検証・AC・採用記録

正式保存先はGitHubのmain。必要な検証後は「検証中」、その版の実際のオンラインACも確認して「正式採用」。[開発規約](../Rule/LibraryDevelopmentRules.md)を適用する。

## 2026-10-12（日本時間）の移行と現行版

基準main: [34eede7d6d376275e1f25bb93403e63b8abd9bc8](https://github.com/primesosu91/algorithm/commit/34eede7d6d376275e1f25bb93403e63b8abd9bc8)。現行ソース版は下表のGit blob SHAで特定する。

従来のAC・正式採用の報告は維持する。過去の最初のAC問題・提出URLと現行版との対応の詳細は未移行であり、この移行だけを理由に採用を取り消さない。問題名・提出IDを記憶から捏造しない。確認できたものから追記する。

MinCostFlowは今回動作に影響する修正を行ったため、必要なローカル検証を通過した後も実際の再ACまでは「検証中」とする。旧版のACを修正版へ流用しない。

| ライブラリ | 現行Git blob SHA | 状態 | AC記録の詳細 |
| --- | --- | --- | --- |
| [RunLengthEncoding](../algorithm/RunLengthEncoding.cpp) | `2490c1cdd542e3b0eee31fc02180f7e0e72cfae2` | 従来の採用状態を維持 | 過去の問題・提出詳細は未移行 |
| [LazySegmentTree](../data_structure/LazySegmentTree.cpp) | `73ca088f5d8446329dd2a85ab82a41c15e94fa3b` | 従来の採用状態を維持 | 過去の問題・提出詳細は未移行 |
| [OrderedSet](../data_structure/OrderedSet.cpp) | `d5e69129b4e8361fc496f7c469fa2bf6fda4bf6b` | 従来の採用状態を維持 | 過去の問題・提出詳細は未移行 |
| [SegmentTree](../data_structure/SegmentTree.cpp) | `03ff41bbb1f74eb970106545266be594a043dc13` | 従来の採用状態を維持 | 過去の問題・提出詳細は未移行 |
| [UnionFind](../data_structure/UnionFind.cpp) | `52213a1f40c4239411e43aacb768b17e299ee9f0` | 従来の採用状態を維持 | 過去の問題・提出詳細は未移行 |
| [ConvexPolygon](../geometry/ConvexPolygon.cpp) | `befd23138399b2f46a87732bf52ecea24e6537e3` | 従来の採用状態を維持 | 過去の問題・提出詳細は未移行 |
| [BellmanFord](../graph/BellmanFord.cpp) | `604849c233489ba68cfd05b4161d08b9fc911b58` | 従来の採用状態を維持 | 過去の問題・提出詳細は未移行 |
| [CycleDetection](../graph/CycleDetection.cpp) | `c5f5544dc440c301083a3e1a402b0fe69289e48b` | 従来の採用状態を維持 | 過去の問題・提出詳細は未移行 |
| [Dijkstra](../graph/Dijkstra.cpp) | `8f95b4587063347fdf154fcb05f19f19aa8550e5` | 従来の採用状態を維持 | 過去の問題・提出詳細は未移行 |
| [LCA](../graph/LCA.cpp) | `9cc01adcd28abd1c975ca854751cb0bce9b80cc1` | 従来の採用状態を維持 | 過去の問題・提出詳細は未移行 |
| [MaxFlow](../graph/MaxFlow.cpp) | `0efd635e006549d72006e2f61078699e160dc8e5` | 従来の採用状態を維持 | 過去の問題・提出詳細は未移行 |
| [MinCostFlow](../graph/MinCostFlow.cpp) | `eb45f905e889965b10c6730c150ecd3635816d7a` | 検証中（動作修正・再AC待ち） | 過去の問題・提出詳細は未移行 |
| [ProjectSelection](../graph/ProjectSelection.cpp) | `087135952c0e40cdcfcd243c196256488ebf282d` | 従来の採用状態を維持 | 過去の問題・提出詳細は未移行 |
| [SCC](../graph/SCC.cpp) | `a9cf9244049a055edcbaa287c7d3c8d0573cabb2` | 従来の採用状態を維持 | 過去の問題・提出詳細は未移行 |
| [TreeDiameter](../graph/TreeDiameter.cpp) | `af703fa4be1ee9dc465e4b63bf9e15f076a578a5` | 従来の採用状態を維持 | 過去の問題・提出詳細は未移行 |
| [WarshallFloyd](../graph/WarshallFloyd.cpp) | `d788589f3cc2b3b8b48038e57396bc3348611a0a` | 従来の採用状態を維持 | 過去の問題・提出詳細は未移行 |
| [ModInt](../math/ModInt.cpp) | `1e654cdc63d8eccffdf1066bebc13d5c43091db9` | 従来の採用状態を維持 | 過去の問題・提出詳細は未移行 |
| [Prime](../math/Prime.cpp) | `b9dc888e30d593709accc240a459777a363c40f9` | 従来の採用状態を維持 | 過去の問題・提出詳細は未移行 |

## 今回の検証

全18回帰テストは通常実行とASan/UBSan実行で通過。テンプレートとスニペットの展開内容も確認済み。詳細な入力範囲・環境上の制約は[DevelopmentLog.md](DevelopmentLog.md)と[tests/README.md](../tests/README.md)に記録した。構文確認・ローカルテスト・GitHub Actionsは実際のオンラインACではない。

## 新規・変更時に残す内容

- ソースパス、対象blob SHA、基準コミットまたは確定済みコミット。
- 検証日、テストファイル、入力範囲・seed・結果・未確認事項。
- 検証中／正式採用の状態と、その根拠。
- 最初にACした問題1件の名前・URL。分かる場合は提出URL・ID。
- ACの確認方法（ユーザー報告／直接確認）と、ACに使ったソース版。
- 動作変更後の再検証・再AC。コメント等だけなら動作変更なしの確認。

AIが依頼・AC報告を受けて更新し、GitHubへ反映する。ユーザーが別の記録ファイルやコマンドを管理する必要はない。未完了の検証を通過済みと報告しない。
