# TicTacToe Game

## 1. Introduction

### 1.1 これは何？

以前、Python で作成した、コンソールで動作する五目並べを C 言語で書き直してみたもの。
https://github.com/ZawaPaP/TicTacToe

### 1.2 目的

C 言語は、多くのプログラミング言語の基盤となっており、またメモリやポインタなどを意識してコードを書く必要がある。
C 言語を実際に書いてみることでコンピュータサイエンスの基礎を理解することが目標。

2. Product Features
   コンソールでの動作 (ネットワークはなし)
   9x9 のマスでの五目並べ

3. 機能要件

- Display game board
- Display game status
- Display player's turn
- Accept console input
- Accept game reset / exit
- Player vs CPU
- Select the player turn (if vs CPU)

4. 非機能要件
   Python のプロジェクトでできていたことが一通りできること。
   ユニットテスト (カバレッジ 60%+)

## 2.使い方

コンパイルコマンド一覧

```
make clean
// srcファイルの *.o ファイルを消去

make all
// コンパイル

make test
// ユニットテストコンパイル & 実行
```

## 3.以下学んだことのメモ。

- `Makefile` はコンパイルコマンドを自動化するためのもの。また実際のコンパイル動作を記述するファイル。

- C 言語はコンパイルしてから実行する。
  コンパイルの流れ

```
ソースコード(.c) -> オブジェクトファイル(.o) -> 実行ファイル
```

- const の意味
  例えば、文字列を扱うとき、

```
char *str = "abc";
```

とかくと、str は書き換えが可能な、配列のポインタとなる。
これは、意図しない書き換えをしてしまう可能性がある。

```
const char *str = "abc"
```

とかくと、読み取り専用となり、書き換えができなくなる。

#### メモリ

- スタックメモリは、関数のスコープが終了すると自動的に解放される。
- ヒープメモリに `malloc`, `realloc` などで動的メモリを割り当てた場合は解放が必要。
