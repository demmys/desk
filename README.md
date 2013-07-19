Desk
==========
demmyによるJVM上で動作するHaskellライクな純粋関数型言語Deskのコンパイラ

##動作確認済み環境##
+ Intel Core i5
+ Mac OS X Lion(10.7.5)
+ gcc version 4.2.1 LLVM build
+ Java version 1.6.0_51

##使用方法##
`make`によって生成される`desk`がDeskコンパイラ本体です。  
DeskではInt型(32bit符号付き整数)以外をサポートしていないため、Hello Worldプログラムを走らせることはできませんが、一般的な電卓が実行できる以下のようなコードをコンパイルすることで動作確認を行うことができます。

```
/*
 * Sample.desk
 */
// this is Desk sample program

main = (314 + 15 - 92 / 6 * 5) % 3;
```

DeskコンパイラはソースコードをJava言語と同じ`.class`ファイルへコンパイルします。そのため、Deskの実行にはJava仮想マシンが必要ですが、逆に言えば、Java仮想マシンが用意されている全ての環境でDeskによってコンパイルしたプログラムを実行することができます。  
Deskの実行はjavaコマンドを用いて以下のように行います。

```
$ vim Sample.desk
(上記コードを記入)
$ desk Sample.desk
$ java Sample
2
```

##目指す言語の特性##
+ 純粋関数型
+ 強い静的型付け
+ (非正格評価) 考え中...
+ 強力な型推論
+ 多相型
+ 詳細なコンパイルエラー  
(それってHaskellじゃ...)
+ JVM上で動くバイトコードを生成

##現在の仕様##
main関数のみ宣言可能。  
Cと同じ意味合いの二項演算子`+`、`-`、`*`、`/`、`%`及び単項演算子`-`、優先度を上げる`(`、`)`を使用可能。  
演算にはInt型(32ビット符号付き整数)のみを使用可能。

##次にやること##
+ main以外のメソッドとその呼び出しを構文解析できるようにする
+ main以外のメソッドをクラスファイルに吐き出せるようにする
+ if文を構文解析できるようにする
+ this構文の実装を行う
+ Deskでいくつかのベンチマークコードを実装する

##中身##
+ `ファイル名` :  
ファイルの説明

###utils/###
+ `storage.h` :  
ストレージ方式を用いたメモリ管理のための構造体、関数プロトタイプの宣言ファイル

+ `storage.c` < `storage.h` :  
ストレージ方式を用いたメモリ管理のための関数実装ファイル

+ `error.h` :  
エラーを表現するための列挙型定数、関数プロトタイプの宣言ファイル

+ `error.c` < `error.h` :  
エラーを実現するための関数、エラー文実装ファイル

###compile/###
+ `compiler.h` :  
Compiler構造体を中心とする、構文木生成のための構造体、列挙型定数、関数プロトタイプの宣言ファイル

+ `compiler.c` :  
Compiler構造体関係の関数実装、静的なコンパイラ型変数の宣言ファイル

+ `create.h` :  
構文解析中に使用するメソッドのプロトタイプ宣言ファイル

+ `create.c` :  
構文解析中に使用する構文に対応したオブジェクトを生成する関数の実装ファイル

+ `desk.l` :  
一応型や識別子も解析するlexファイル

+ `desk.y` :  
mainとその右辺の定数式のみ解析できるyaccファイル

###generate/###
+ `base.h` :  
クラスファイル内で用いられる識別子のtypedef宣言ファイル

+ `constant_pool.h` :  
ConstantInfo構造体を中心とする、コンスタントプールを実現するための構造体、列挙型定数、関数プロトタイプ宣言ファイル

+ `constant_pool.c` :  
ClassFile構造体内のコンスタントプールを実装するための関数定義ファイル

+ `attributes.h` :  
AttributeInfo構造体を中心とする、アトリビュートを実現するための構造体、列挙型定数、関数プロトタイプ宣言ファイル

+ `attributes.c` :  
ClassFile構造体やCodeアトリビュート内のアトリビュートを実装するための関数定義ファイル

+ `definition.h` :  
Definition構造体を中心とする、メソッド・フィールド・インターフェースを実現するための構造体、列挙定数、関数プロトタイプ宣言ファイル

+ `definition.c` :  
ClassFile構造体内のメソッド・フィールド・インターフェースを実装するための関数定義ファイル

+ `opcode.h` :  
OpcodeInfo構造体を中心とする、JVMのオペコードを生成するための構造体宣言ファイル

+ `opcode.c` :  
JVMのオペコード情報を保持した配列の実装ファイル

+ `classfile.h` :  
ClassFile構造体を中心とする、クラスファイルのメモリ内表現を生成するのための構造体、列挙型定数、関数プロトタイプの宣言ファイル

+ `generator.h` :  
ClassFile構造体のメンバを生成するための構造体、関数プロトタイプの宣言ファイル

+ `generator.c` :  
ClassFile構造体のメンバを生成するための関数実装ファイル

###emit/###
+ `emitter.h` < `generator.h` :  
クラスファイルを生成するための関数プロトタイプ宣言ファイル

+ `emitter.c` < `emitter.h` :  
クラスファイルを生成するための関数実装ファイル

###./###
+ `main.c` < `emitter.h`, `y.tab.h` :  
実際に各生成フェーズを実行するコンパイラのメインエントリポイント実装ファイル

+ `Test.desk` :  
最終的にコンパイルしたいファイルの簡単なもの(四則演算のみ)

+ `Test2.desk` :  
最終的にコンパイルしたいファイルの簡単なもの(関数呼び出しあり)

+ `Test3.desk` :  
最終的にコンパイルしたいエラトステネスの篩プログラム

##参考##
前橋和弥 (2009). プログラミング言語を作る. 東京:株式会社技術評論社  
Tim Lindholm/Frank Yellin/村上 雅章(訳) (2001). Java仮想マシン仕様 第2版. 東京:株式会社ピアソン・エデュケーション
