// コードジェネレータ
#include "9cc.h"

// アセンブリを出力する（再帰関数）
static void gen(Node *node) {
  if (node->kind == ND_NUM) {
    printf("\tpush %d\n", node->val);
    return;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("\tpop rdi\n");
  printf("\tpop rax\n");

  switch (node->kind) {
    case ND_ADD:
      printf("\tadd rax, rdi\n");
      break;
    case ND_SUB:
      printf("\tsub rax, rdi\n");
      break;
    case ND_MUL:
      printf("\timul rax, rdi\n");
      break;
    case ND_DIV:
      printf("\tcqo\n");
      printf("\tidiv rdi\n");
      break;
    case ND_EQ:
      printf("\tcmp rax, rdi\n");
      printf("\tsete al\n");
      printf("\tmovzb rax, al\n");
      break;
    case ND_NE:
      printf("\tcmp rax, rdi\n");
      printf("\tsetne al\n");
      printf("\tmovzb rax, al\n");
      break;
    case ND_LT:
      printf("\tcmp rax, rdi\n");
      printf("\tsetl al\n");
      printf("\tmovzb rax, al\n");
      break;
    case ND_LE:
      printf("\tcmp rax, rdi\n");
      printf("\tsetle al\n");
      printf("\tmovzb rax, al\n");
      break;
  }

  printf("\tpush rax\n");

}

void codegen(Node *node) {
  // アセンブリの前半部分を出力
  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  // 抽象構文木を下りながらコード生成
  gen(node);

  // スタックストップに式全体の値が残っているはずなので
  // それをRAXにロードしてから関数からの返り値とする
  printf("\tpop rax\n");
  printf("\tret\n");
}