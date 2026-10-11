# algorithm

C++23向けの自作競技プログラミングライブラリ。

ライブラリ本体は [library/](library/README.md)、規約・手順・APIカタログは [Rule/](Rule/README.md) にまとめる。

| フォルダ | 内容 |
| --- | --- |
| [library/](library/README.md) | カテゴリ別のライブラリ本体と目次 |
| [Rule/](Rule/README.md) | 規約、設計・開発手順、命名・APIカタログ |
| [tests/](tests/README.md) | 対応する回帰テストと検証の説明 |
| [templates/](templates/) | AtCoderテンプレート |
| [snippets/](snippets/) | テンプレートと全ライブラリの生成スニペット |
| [tools/](tools/) | 生成・整合性確認・検証スクリプト |
| [records/](records/DevelopmentLog.md) | 変更理由・対象版・検証結果・未確認事項 |

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

完了済みの開発チャットは削除してよい。チャットにしかない未完了の仕様・承認・作業差分は、削除前にGitHubへ保存する。確認方法は [運用ガイド](Rule/CompetitiveProgrammingGuide.md) 第6節に従う。

Workで欲しい機能や修正内容を伝え、提示された関数名と仕様を承認すると、AIが配置先の選択・フォルダ作成、実装・検証・スニペット生成・資料更新・コミット・GitHub反映まで行う。push・PRごとにGitHub Actionsが配置、カタログ、スニペット、C++23と保存済みテストを確認する。

オンラインジャッジへの提出・AC・再ACは開発の必須工程に含めない。必要なテストと整合性確認は継続し、結果と未確認事項を作業記録と完了報告に残す。

ライブラリの一覧とソースへのリンクは [library/README.md](library/README.md) を参照する。
