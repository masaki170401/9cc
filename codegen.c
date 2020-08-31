// コードジェネレータ
#include "9cc.h"

// 指定されたノードのアドレスをスタックにプッシュする
static void gen_addr(Node *node) {
  if (node->kind == ND_VAR) {
    printf("\tlea rax, [rbp-%d]\n", node->var->offset);
    printf("\tpush rax\n");
    return;
  }
  error("左辺値ではありません");
}

static void load() {
  printf("\tpop rax\n");
  printf("\tmov rax, [rax]\n");
  printf("\tpush rax\n");
}

static void store() {
  printf("\tpop rdi\n");
  printf("\tpop rax\n");
  printf("\tmov [rax], rdi\n");
  printf("\tpush rdi\n");
}

// アセンブリを出力する（再帰関数）
static void gen(Node *node) {
  switch (node->kind) {
    case ND_NUM:
      printf("\tpush %ld\n", node->val);
      return;
    case ND_EXPR_STMT:
      gen(node->lhs);
      printf("\tadd rsp, 8\n");
      return;
    case ND_VAR:
      gen_addr(node);
      load();
      return;
    case ND_ASSIGN:
      gen_addr(node->lhs);
      gen(node->rhs);
      store();
      return;
    case ND_RETURN:
      gen(node->lhs);
      printf("\tpop rax\n");
      printf("\tjmp .L.return\n");
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

void codegen(Function *prog) {
  // アセンブリの前半部分を出力
  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  // プロローグ
  printf("\tpush rbp\n");
  printf("\tmov rbp, rsp\n");
  printf("\tsub rsp, %d\n", prog->stack_size);

  // 抽象構文木を下りながらコード生成
  for (Node *node = prog->node; node; node = node->next)
    gen(node);

  // エピローグ
  printf(".L.return:\n");
  printf("\tmov rsp, rbp\n");
  printf("\tpop rbp\n");

  printf("\tret\n");
}