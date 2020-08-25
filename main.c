#include "9cc.h"

int main(int argc, char **argv) {
  if (argc != 2 )
    error("%s: 引数の個数が正しくありませ", argv[0]);

  // トークナイズしてパースする
  user_input = argv[1];
  token = tokenize();
  Node *node = expr();

  // 抽象構文木を下りながらコード生成
  codegen(node);

  return 0;
}
