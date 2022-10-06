
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double fib(double);
;
void ___MEMO_fib_save(void);

int main(void) {

  while (1) {
    char buf[100];
    memset(buf, 0, 100);
    printf("func name: ");
    scanf("%s", buf);

    if (strcmp(buf, "exit") == 0) {
      break;
    }

    if (strcmp("fib", buf) == 0) {
      double n;
      printf("1 args > ");
      scanf("%lf", &n);

      double res = fib(n);

      printf("\nres> %.16f\n", res);
    }
    fflush(stdout);

    if (strcmp(buf, "") == 0) {
      break;
    }
  }
  ___MEMO_fib_save();
}
