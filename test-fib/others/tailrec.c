#include <time.h>
#include <stdio.h>

double fib(double a, double b, double n) {
  if (n == 0) {
    return a;
  }
  return fib(b, a+b, n-1);
}


int main() {
  clock_t s, e;
  for (double i = 0; i <= 2048; i++) {
    s = clock();
    fib(0,1,i);
    e = clock();
    if ((int)i % 25 == 0)
    printf("(%d , %.14f) ",(int)i, ((double) e - s) / CLOCKS_PER_SEC);
  }
}
