# Competitive Programming 共通ルール

## 基本環境

・C++23。主にAtCoderで使用する。
・AtCoder Library（ACL）は原則として使用せず、自作ライブラリを優先する。
・問題文・制約・提示情報にない条件を勝手に仮定しない。

## Workとチャット

・新規ライブラリ、既存ライブラリの修正、規約改訂、環境整備とGitHub更新はWorkで行う。
・新しい依頼は原則新しいWorkチャットで始める。関連する以前の履歴のチャットが残っていれば、そのチャットをWorkで継続してもよい。
・新規チャットでも最新mainの規約・対象ソース・テスト・作業記録から対応する。過去チャットや添付を開始の必須条件にしない。
・完了済みの開発チャットは削除してよい。チャットだけに残る未完了の仕様・承認・作業差分は、削除前にGitHubへ保存する。削除した会話や未保存差分を記憶から復元しない。
・通常チャットで依頼された場合、GitHubを変更する作業はWorkへ引き継ぐ。途中作業の仕様・承認・未保存差分を確認できなければ、記憶から補わない。

## 正式版と作業開始

・正式な保存先は https://github.com/primesosu91/algorithm のmain。
・ソース、規約、カタログ、手順、テンプレート、スニペット、テスト、作業・検証記録をGitHubで管理し、ChatGPT LibraryやProjectにファイルを常設しない。
・作業前にmainの最新コミットを確認し、その版のAGENTS.md、Rule/の該当規約、Rule/NamingAndAPI.md、library/data_structure/UnionFind.cpp、対象・関連ソースを実際に読む。
・開始時に確認コミット・参照ファイル・作業範囲を明示する。
・カタログ・会話・参照コピーと実装が異なれば最新実装を正とする。取得できない内容を記憶から補わない。

## 参照先

・AIの入口: https://github.com/primesosu91/algorithm/blob/main/AGENTS.md
・規約一覧: https://github.com/primesosu91/algorithm/blob/main/Rule/README.md
・開発規約: Rule/LibraryDevelopmentRules.md
・デバッグ: Rule/DebugRules.md
・Project運用: Rule/CompetitiveProgrammingGuide.md
・命名とAPI: Rule/NamingAndAPI.md
・作業手順: Rule/LibraryWorkflow.md
・フォルダ構成と自動配置: Rule/RepositoryLayout.md
・スニペット: Rule/SnippetRules.md
・作業・検証記録: records/DevelopmentLog.md
・共通ルールと該当する専用規約を両方適用する。

## 依頼と承認

・ユーザーは欲しいライブラリ・機能、または修正内容を伝える。
・AIは既存実装を参照し、型名・関数名・引数・返り値・意味・前提・計算量と既存設計との対応を示す。修正ならAPIへの影響も説明する。
・配置先はAIが用途・前提・既存構成から決め、設計案へソース・テストの予定パスと分類理由を併記する。必要なフォルダ作成と配置もAIが行い、ユーザーに操作を求めない。
・本体はlibrary/カテゴリ/へ置く。木を前提とするグラフアルゴリズムはlibrary/tree/、一般のグラフはlibrary/graph/、SegmentTree等の汎用データ構造はlibrary/data_structure/。対応テストはlibrary/以下と同じ相対構成のtests/へ置く。移動時は関連include・目次・カタログ・スニペット・記録を同期する。
・通常の新規作成・ソース変更は、この案へのユーザーの明示的な承認を待つ。
・承認後は、実装、コマンド実行、対象固有のテスト、スニペット生成、カタログ・記録更新、コミット、GitHub反映までAIが完了させる。通常の各工程に再承認や「次へ」を求めない。
・承認した仕様を変える必要が生じたら、新しい案を説明して了承を得る。
・破壊的変更は理由・対象・移行方法を示して具体的な了承を得る。既に了承された変更について同じ承認を求め直さない。

## スニペット

・snippets/competitive_programming.code-snippetsを更新する。
・先頭はtemplates/AtCoder.cppから作る!atcoder。solve()の最初の行へ$0を設定する。
・各ライブラリは冒頭のincludeとusing namespace stdの2行だけを除いた全文。残る空行・改行・コメント・インデントを保つ。
・名前はファイル名の拡張子を除いたもの、prefixは!+名前。項目にはprefixとbodyだけを置く。
・ソース、スニペット、カタログ、必要なテストと記録を原則同じコミットにする。

## 正確性と反映

・正確性を最優先する。不明点を推測で断定しない。
・オンライン提出・AC・再ACは開発の完了条件に含めず、採用状態の個別管理もしない。
・C++23で対象固有のテストと共通チェックを通過させ、関連ファイルを同期してGitHubへ反映する。動作変更後は必要な回帰テストと再検証を行う。
・対象ソース版、検証内容・範囲・結果、未確認事項を作業記録に残す。ローカル検証とGitHub Actionsの結果を区別し、未実施のオンラインACを確認済みと報告しない。
・当該コミットのGitHub Actionsも取得できれば確認し、検証の未完了・失敗、main未反映、Actionsの実行中・失敗・取得不能は明示する。
・ユーザーに通常のGitコマンド入力を求めない。反映直前にmainを再確認し、同時更新を上書きしない。
・許可範囲外の資料削除、履歴削除、強制push、無関係な変更を行わない。
・完了時に変更内容、検証結果と未確認事項、コミットとスニペットのリンク、残る作業を示す。
