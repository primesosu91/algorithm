# Rule — 開発時に読む資料

正式版は [primesosu91/algorithm](https://github.com/primesosu91/algorithm) のmain。作業開始時に最新コミットを特定し、その版を読む。

| 資料 | 役割 |
| --- | --- |
| [LibraryDevelopmentRules.md](LibraryDevelopmentRules.md) | 実装・API・コメント・検証・完了条件・設計承認の規約 |
| [CompetitiveProgrammingGuide.md](CompetitiveProgrammingGuide.md) | Project・チャット・保存先の運用 |
| [DebugRules.md](DebugRules.md) | 不具合の原因調査と必要最小限の修正 |
| [NamingAndAPI.md](NamingAndAPI.md) | 全ライブラリの現在の公開APIと命名上の例外 |
| [LibraryWorkflow.md](LibraryWorkflow.md) | 依頼・承認・自動更新の順序と依頼例 |
| [SnippetRules.md](SnippetRules.md) | テンプレートと全文スニペットの生成仕様 |
| [ProjectInstructions.md](ProjectInstructions.md) | Projectの指示へ一度貼り付ける全文 |

AIの入口は [AGENTS.md](../AGENTS.md)。実装の代表例は [UnionFind.cpp](../data_structure/UnionFind.cpp)。変更理由・対象ソース版・検証結果・未確認事項は [DevelopmentLog.md](../records/DevelopmentLog.md) で管理する。オンラインACと採用状態の個別管理は必須工程に含めない。

既存のdocsは2026-10-12（日本時間）の明示的な移行依頼により廃止した。過去の内容はGit履歴から参照できる。規約・ソースをChatGPTへ常設アップロードする運用は行わない。
