Desk
==========
demmyによるHaskellライクな言語Deskのコンパイラです。

###目指す言語の特性###
+ 純粋関数型
+ 強い静的型付け
+ 非正格評価
+ 強力な型推論
+ 多相型
+ 詳細なコンパイルエラー  
(それってHaskellじゃ...)
+ JVM上で動くバイトコードを生成

###現在の状況###
正しいファイルはそのまま何も起こらず、間違ったファイルだとコンパイルエラーだといってエラー行番号とエラーの種類(mainの重複、構文解析、字句解析)を吐いて終了するものができた。

###現在の仕様###
`main`関数のみ宣言可能。  
Cと同じ意味合いの二項演算子`+`、`-`、`*`、`/`、`%`及び単項演算子`-`、優先度を上げる`(`、`)`を使用可能。  
型にはChar型(Cのchar)、Int型(Cのint)、Float型(Cのdouble)があり、それぞれ`'a'`、`128`、`3.14`のように定数を使用可能。  
今のところはどのような型に対してもすべての演算子を適用できる。(弱い型付け)

###次にやること###
+ 定数間の演算も強い型付にする
+ generate.cをつくり、メモリ上の構文木からJVMのニーモニックもどきをstdoutにプリントする
+ それぞれの状況に合わせてしっかりとコンパイルエラーを出す

###中身###
+ `Makefile` :  
`make`でコンパイル、`make help`でその他の使い方の確認ができる
+ `desk.h` :  
列挙型定数、構造体、関数プロトタイプの宣言ファイル
+ `desk.l` :  
一応型や識別子も解析するlexファイル
+ `desk.y` :  
mainとその右辺の定数式のみ解析できるyaccファイル
+ `compiler.c` :  
コンパイラ関係の関数実装、静的なコンパイラ型変数の宣言ファイル
+ `create.c` :  
構文解析中に使用する構文に対応したオブジェクトを生成する関数の実装ファイル
+ `storage.h` :  
メモリ管理のための構造体、関数プロトタイプの宣言ファイル
+ `storage.c` :  
メモリ管理のための関数実装ファイル
+ `test.desk` :  
最終的にコンパイルしたいファイルの簡単なもの(四則演算のみ)
+ `test2.desk` :  
最終的にコンパイルしたいファイルの簡単なもの(関数呼び出しあり)
+ `test3.desk` :  
最終的にコンパイルしたいエラトステネスの篩プログラム

###参考###
前橋和弥 (2009). プログラミング言語を作る. 東京:株式会社技術評論社
