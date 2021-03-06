#define _GNU_SOURCE
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**************************/
/*       tokenize.c       */
/**************************/

// トークンの種類
typedef enum {
  TK_RESERVED,  // 記号
  TK_IDENT,     // 識別子
  TK_NUM,       // 整数トークン
  TK_EOF,       // 入力の終わりを表すトークン
} TokenKind;

// トークン型
typedef struct Token Token;
struct Token {
  TokenKind kind; // トークンの型
  Token *next;    // 次の入力トークン
  long val;        // kindがTK_NUMの場合、その数値
  char *str;      // トークン文字列
  int len;        // トークンの長さ
};

void error(char *fmt, ...); // エラーを報告するための関数。printfと同じ引数を取る
void error_at(char *loc, char *fmt, ...);   // エラー箇所を報告する
bool consume(char *op); // 次のトークンが期待している記号のときには、トークンを１つ読み進めて真を返す。それ以外の場合には偽を返す。
Token *consume_ident(void);   // 現在のトークンが識別子の場合、それを消費する
void expect(char *op);  // 次のトークンが期待している記号のときには、トークンを１つ読み進める。それ以外のときにはエラーを報告する。
long expect_number(void);    // 次のトークンが数値の場合、トークンを1つ読み進めてその数値を返す。それ以外の場合にはエラーを報告する。
bool at_eof(void);  // トークンの種類が終端(TK_EOF)の場合、trueを返す
Token *tokenize(void);  // 入力文字列'user_input'をトークナイズしてそれを返す

extern char *user_input;    // 入力するプログラム
extern Token *token;    // 現在注目しているトークン

/**************************/
/*        parse.c         */
/**************************/

// ローカル変数の型
typedef struct Var Var;
struct Var {
  Var *next; // 次の変数かNULL
  char *name; // 変数の名前
  int offset; // RBPからのオフセット
};

// 抽象構文木のノードの種類
typedef enum {
  ND_ADD,       // +
  ND_SUB,       // -
  ND_MUL,       // *
  ND_DIV,       // /
  ND_EQ,        // ==
  ND_NE,        // !=
  ND_LT,        // <
  ND_LE,        // <=
  ND_ASSIGN,    // =
  ND_RETURN,    // "return"
  ND_EXPR_STMT, // 式(expression) ステートメント(statement)
  ND_VAR,       // ローカル変数
  ND_NUM,       // 整数
} NodeKind;

// 抽象構文木のノードの型
typedef struct Node Node;
struct Node {
  NodeKind kind; // ノードの型
  Node *next;    // 次のノード
  Node *lhs;     // 左辺
  Node *rhs;     // 右辺
  Var *var;     // kindがND_VARの場合のみ使う
  long val;       // kindがND_NUMの場合のみ使う
};

typedef struct Function Function;
struct Function {
  Node *node;
  Var *locals;
  int stack_size;
};

Function *program(void);

/**************************/
/*       codegen.c        */
/**************************/

// アセンブリを出力する
void codegen(Function *prog);
