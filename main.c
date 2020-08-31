#include "9cc.h"

int main(int argc, char **argv) {
  if (argc != 2 )
    error("%s: 引数の個数が正しくありませ", argv[0]);

  // トークナイズしてパースする
  user_input = argv[1];
  token = tokenize();
  Function *prog = program();

  //
  int offset = 0;
  for (Var *var = prog->locals; var; var = var->next) {
    offset += 8;
    var->offset = offset;
  }
  prog->stack_size = offset;
  
  // 抽象構文木を下りながらコード生成
  codegen(prog);

  return 0;
}
