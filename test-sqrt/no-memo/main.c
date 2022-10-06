
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double diff(double, double);
double mysqrt(double);

int main(void) {


  for (int i = 0; i < 50000; i++) {
    if (i % 1000 == 0) {printf("%d\n", i);}
    double r = (double) (rand() % 2000);
    mysqrt(r);
  }

}