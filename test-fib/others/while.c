#include <stdio.h>
#include <time.h>
double fib(double n) {
    double b = 0;
    double a = 1;
    double temp;
    while (n) {
        temp = a;
        a = a + b;
        b = temp;
        n--;
    }
    return a;
}


int main() {
  clock_t s, e;
  for (double i = 0; i <= 2048; i++) {
    s = clock();
    fib(i);
    e = clock();
    if ((int)i % 25 == 0)
    printf("(%d , %.14f) ",(int)i, ((double) e - s) / CLOCKS_PER_SEC);
  }
}
