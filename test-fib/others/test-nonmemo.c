#include <stdio.h>
#include <time.h>
double fib(double n) {
    if (n <= 1) {
        return n;
    }
    return fib(n - 1) + fib(n - 2);
}


int main() {
  clock_t s, e;
  for (double i = 0; i <= 40; i++) {
    s = clock();
    fib(i);
    e = clock();
    if ((int)i % 10 == 0)
    printf("(%d , %.14f) ",(int)i, ((double) e - s) / CLOCKS_PER_SEC);
  }
}
