# スニペット生成規約

## 1. 原本と生成物

テンプレートの原本は [templates/AtCoder.cpp](../templates/AtCoder.cpp)。各ライブラリの原本はカテゴリ別の `.cpp`。生成物は [competitive_programming.code-snippets](../snippets/competitive_programming.code-snippets)。ユーザーが提示したAtCoderテンプレートをそのまま使う。

| 項目 | 要件 |
| --- | --- |
| 最初の項目 | 名前 `atcoder`、prefix `!atcoder`、テンプレート全文 |
| 最終カーソル | `solve()`の最初の行の4スペースの直後に `$0` を1つ置く |
| ライブラリ名 | `.cpp` を除いたファイル名。型を持たないPrime等もファイル名で命名 |
| prefix | `!` と名前を連結する。別名や複数prefixは作らない |
| body | `#include <bits/stdc++.h>` と `using namespace std;` の2行だけを除いた全文 |
| 保持する内容 | 残る空行、末尾改行、インデント、コメント、1行の長さ |
| 項目のキー | `prefix` と `body` だけ。description、scope、isFileTemplate等は付けない |
| 順序 | atcoderが先頭、以後はライブラリのパス順で決定。新規ソースも自動収録 |

ライブラリの冒頭2行が想定と違う場合、推測で削らず生成を失敗させて確認する。同じ名前のソースが複数ある場合も重複prefixを作らずエラーにする。

## 2. 展開文字列の保持

JSONのエスケープとスニペット展開のエスケープを区別する。コード中のバックスラッシュやドル記号を文字として保持し、テンプレートの指定位置にだけ有効な `$0` を置く。JSON上の表現がエスケープされても、展開したC++は原本の全文と一致するようにする。

VS Codeの[公式スニペット仕様](https://code.visualstudio.com/docs/editing/userdefinedsnippets)を参照する。各ライブラリへ入力用プレースホルダーや独自コメントを追加しない。

## 3. AIが毎回行う更新

```bash
python3 tools/generate_snippets.py
python3 tools/check_repository.py
```

ユーザーに実行を求めず、承認済みの実装・修正後にAIが行う。確認はJSONのキー・名前・prefix・順序・カーソル位置と、展開文字列がソースに一致することを対象とする。

共通チェックは、テンプレート単体およびテンプレートに各ライブラリを挿入したC++23コードの構文も確認する。これだけでテンプレート型の全組合せや全アルゴリズムの正しさを認定しない。

ソース、スニペット、APIカタログ、必要なテスト・記録は同じコミットにまとめる。手書きでスニペットだけを直して原本との差を作らない。

## 4. 手元のVS Codeに取り込む

1. 完了報告の `competitive_programming.code-snippets` をダウンロードする。
2. VS Codeで `Ctrl+Shift+P` を押し、`Snippets: Configure Snippets`（スニペットの構成）を選ぶ。
3. 全体で使う場合は `New Global Snippets file` を選び、名前を `competitive_programming` とする。既に同名の登録先がある場合はそのファイルを開く。
4. 生成された編集画面の内容を、ダウンロードしたファイルの全文に置き換えて保存する。
5. C++ファイルで `!atcoder` を入力し、候補を選んで展開する。solveの最初の行にカーソルが来ることを確認する。
6. ライブラリはsolveの外、通常はsolveの上で `!UnionFind` などを展開する。

このファイルはscopeを指定しないため全言語の候補に入る。C++で使用する。Tabで展開したい場合はVS Codeの `editor.tabCompletion` を `on` に設定できる。候補が出ない場合は `Ctrl+Space` または `Snippets: Insert Snippet` から選ぶ。

GitHubのスニペットはAIが毎回更新する。Workから手元のVS Codeを直接変更できない環境では、更新版を使うときに登録済みスニペットの全文を置き換える操作だけユーザーが行う。GitHubの更新だけで手元にも自動反映されたと報告しない。

改訂日: 2026-10-12（日本時間）。
