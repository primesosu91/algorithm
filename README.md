# algorithm

C++23向けの自作競技プログラミングライブラリ。

## 管理資料

- [命名・APIカタログ](Rule/NamingAndAPI.md)
- [開発・更新の手順](Rule/LibraryWorkflow.md)
- [フォルダ構成と自動配置](Rule/RepositoryLayout.md)
- [ライブラリ開発規約](Rule/LibraryDevelopmentRules.md)
- [Competitive Programming 運用ガイド](Rule/CompetitiveProgrammingGuide.md)
- [DebugRules](Rule/DebugRules.md)
- [AIの作業指示](AGENTS.md)
- [Projectの指示に設定する全文](Rule/ProjectInstructions.md)
- [開発作業と検証結果の記録](records/DevelopmentLog.md)
- [スニペット生成規約とVS Codeへの取り込み方](Rule/SnippetRules.md)
- [現在の全スニペット](snippets/competitive_programming.code-snippets)
- [AtCoderテンプレート](templates/AtCoder.cpp)
- [自動テストの説明](tests/README.md)

ソース・規約・APIカタログ・テスト・開発作業と検証結果の記録の正式版は、このリポジトリのmain。ChatGPT側への常設アップロードは不要。

新規作成・修正・規約改訂は原則新しいWorkチャットで依頼する。関連する以前のチャットが残っていれば、Workでその履歴を引き継いで続けてもよい。新規チャットでも、AIが最新mainの規約・実装・作業記録から対応する。

Workで欲しい機能や修正内容を伝え、提示された関数名と仕様を承認すると、AIが配置先の選択・フォルダ作成、実装・検証・スニペット生成・資料更新・コミット・GitHub反映まで行う。push・PRごとにGitHub Actionsが配置、カタログ、スニペット、C++23と保存済みテストを確認する。

オンラインジャッジへの提出・AC・再ACは開発の必須工程に含めない。必要なテストと整合性確認は継続し、結果と未確認事項を作業記録と完了報告に残す。

## Data Structure

- [UnionFind](data_structure/UnionFind.cpp)
- [OrderedSet](data_structure/OrderedSet.cpp)
- [SegmentTree](data_structure/SegmentTree.cpp)
- [LazySegmentTree](data_structure/LazySegmentTree.cpp)

## Graph

- [CycleDetection](graph/CycleDetection.cpp)
- [Dijkstra](graph/Dijkstra.cpp)
- [BellmanFord](graph/BellmanFord.cpp)
- [WarshallFloyd](graph/WarshallFloyd.cpp)
- [SCC](graph/SCC.cpp)
- [MaxFlow](graph/MaxFlow.cpp)
- [MinCostFlow](graph/MinCostFlow.cpp)
- [ProjectSelection](graph/ProjectSelection.cpp)

## Tree

- [LCA](tree/LCA.cpp)
- [TreeDiameter](tree/TreeDiameter.cpp)

## Math

- [ModInt](math/ModInt.cpp)
- [Prime](math/Prime.cpp)

## Geometry

- [ConvexPolygon](geometry/ConvexPolygon.cpp)

## Algorithm

- [RunLengthEncoding](algorithm/RunLengthEncoding.cpp)
