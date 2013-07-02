Desk
==========
demmyによるHaskellライクな言語Deskのコンパイラです。

###目指す言語の特性###
+ 純粋関数型
+ 強い静的型付け
+ (非正格評価) 考え中...
+ 強力な型推論
+ 多相型
+ 詳細なコンパイルエラー  
(それってHaskellじゃ...)
+ JVM上で動くバイトコードを生成

###現在の状況###
正しいファイルを渡すとjavapできるだけでmain関数の中身は反映されていないクラスファイルが吐かれ、間違ったファイルだとコンパイルエラーだといってエラー行番号とエラーの種類を吐いて終了するものができた。
生成されたクラスファイルはjavap -vで中身を確かめられる。
ただし、現在は強制スワップをしているため、変数がリトルエンディアンでない環境ではうまくファイルを生成できない。

###現在の実装###
コンパイルの全体はコンパイラを表現する構造体Compilerのメンバを生成するyyparseメソッドによるフェーズとクラスファイルを表現する構造体ClassFileのメンバを生成するgenerateメソッドによるフェーズ、バイナリのクラスファイル自体を生成するemitメソッドによるフェーズの3つにわかれている。
コンスタントプールなどの連続した値の保持には先頭のprevメンバが末尾のnextメンバとなる双方向循環リストによって表現されている。

###現在の仕様###
main関数のみ宣言可能。  
Cと同じ意味合いの二項演算子`+`、`-`、`*`、`/`、`%`及び単項演算子`-`、優先度を上げる`(`、`)`を使用可能。  
型にはChar型(Cのchar)、Int型(Cのint)、Float型(Cのdouble)があり、それぞれ`'a'`、`128`、`3.14`のように定数を使用可能。  
今のところはどのような型に対してもすべての演算子を適用できる。(弱い型付け)

###次にやること###
+ コンストラクタをメソッドとしてCodeアトリビュートを用いて表現する
+ 与えられたmain関数の内容をJVMのニーモニックに落とせるようにする
+ メソッドをクラスファイルに吐き出せるようにする
+ 定数間の演算も強い型付にする
+ main以外のメソッドとその呼び出しを構文解析できるようにする
+ main以外のメソッドをクラスファイルに吐き出せるようにする
+ if文を構文解析できるようにする
+ this構文の実装を行う
+ Deskでいくつかのベンチマークコードを実装する

###中身###
+ `ファイル名 < includeヘッダファイル` :  
ファイルの説明

#utils/#
+ `storage.h` :  
ストレージ方式を用いたメモリ管理のための構造体、関数プロトタイプの宣言ファイル

+ `storage.c < storage.h` :  
ストレージ方式を用いたメモリ管理のための関数実装ファイル

+ `error.h` :  
エラーを表現するための列挙型定数、関数プロトタイプの宣言ファイル

+ `error.c < error.h` :  
エラーを実現するための関数、エラー文実装ファイル

#compile/#
+ `compiler.h < storage.h, error.h` :  
Compiler構造体を中心とする、構文木生成のための構造体、列挙型定数、関数プロトタイプの宣言ファイル

+ `compiler.c < compiler.h` :  
Compiler構造体関係の関数実装、静的なコンパイラ型変数の宣言ファイル

+ `create.h < compiler.h` :  
構文解析中に使用するメソッドのプロトタイプ宣言ファイル

+ `create.c < create.h` :  
構文解析中に使用する構文に対応したオブジェクトを生成する関数の実装ファイル

+ `desk.l < create.h` :  
一応型や識別子も解析するlexファイル

+ `desk.y < create.h` :  
mainとその右辺の定数式のみ解析できるyaccファイル

#generate#
+ `classfile_base.h` :  
クラスファイル内で用いられる識別子のtypedef宣言ファイル

+ `classfile_constant_pool.h < classfile_base.h` :  
ConstantInfo構造体を中心とする、コンスタントプールを実現するための構造体、列挙型定数、関数プロトタイプ宣言ファイル

+ `classfile_attributes.h < classfile_base.h` :  
AttributeInfo構造体を中心とする、アトリビュートを実現するための構造体、列挙型定数、関数プロトタイプ宣言ファイル

+ `code.h < classfile_base.h` :  
これから使用するOpcode構造体を中心とする、JVMのオペコードを生成するための構造体宣言ファイル

+ `code.c < code.h:  
これから使用するJVMのオペコード情報を保持した配列の実装ファイル

+ `classfile.h < storage.h, error.h, classfile_constant_pool.h, classfile_attributes.h` :  
ClassFile構造体を中心とする、クラスファイルのメモリ内表現を生成するのための構造体、列挙型定数、関数プロトタイプの宣言ファイル

+ `generator.h < create.h, classfile.h` :  
ClassFile構造体のメンバを生成するための構造体、関数プロトタイプの宣言ファイル

+ `generator.c < generator.h` :  
ClassFile構造体のメンバを生成するための関数実装ファイル

#emit/#
+ `emitter.h < generator.h` :  
クラスファイルを生成するための関数プロトタイプ宣言ファイル

+ `emitter.c < emitter.h` :  
クラスファイルを生成するための関数実装ファイル

#./#
+ `main.c < emitter.h, y.tab.h` :  
実際に各生成フェーズを実行するコンパイラのメインエントリポイント実装ファイル

+ `Test.desk` :  
最終的にコンパイルしたいファイルの簡単なもの(四則演算のみ)

+ `Test2.desk` :  
最終的にコンパイルしたいファイルの簡単なもの(関数呼び出しあり)

+ `Test3.desk` :  
最終的にコンパイルしたいエラトステネスの篩プログラム

###参考###
前橋和弥 (2009). プログラミング言語を作る. 東京:株式会社技術評論社  
Tim Lindholm/Frank Yellin/村上 雅章(訳) (2001). Java仮想マシン仕様 第2版. 東京:株式会社ピアソン・エデュケーション
