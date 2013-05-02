Desk
==========
demmyによるHaskellライクな純粋関数型言語(desk)のコンパイラです。

目標
----------
Turing完全な純粋関数型言語

いま
----------
よさげなトークンから構成されたファイルを字句解析できる

中身
----------
・Makefile  
・desk_.l  
    -> とりあえずコンパイルできるようにした字句解析テスト用lexファイル  
・desk.l  
    -> 最終的になりそうな形のlexファイル  
・test.desk  
    -> 最終的にコンパイルしたいファイルの簡単なもの(四則演算のみ)  
・test2.desk  
    -> 最終的にコンパイルしたいファイルの簡単なもの(関数呼び出しあり)  
・test3.desk  
    -> 最終的にコンパイルしたいエラトステネスの篩プログラム  
