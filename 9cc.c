#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 2 ) {
    fprintf(stderr, "引数の個数が正しくありませ\n");
    return 1;
  }

  printf(".intel_syntax noprefix\n");
  printf(".globl main\n");
  printf("main:\n");
  printf("\t mov rax, %d\n", atoi(argv[1]));
  printf("\t ret\n");
  return 0;
}
