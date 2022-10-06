#include <time.h>
#include <stdio.h>
void multiply(double F[2][2], double M[2][2]);
 
/* Helper function that calculates F[][] raise to the power n and puts the
  result in F[][]
  Note that this function is designed only for fib() and won't work as general
  power function */
void power(double F[2][2], double n);
 
double fib(double n)
{
  double F[2][2] = {{1,1},{1,0}};
  if (n == 0)
      return 0;
  power(F, n-1);
 
  return F[0][0];
}
 
void multiply(double F[2][2], double M[2][2])
{
  double x =  F[0][0]*M[0][0] + F[0][1]*M[1][0];
  double y =  F[0][0]*M[0][1] + F[0][1]*M[1][1];
  double z =  F[1][0]*M[0][0] + F[1][1]*M[1][0];
  double w =  F[1][0]*M[0][1] + F[1][1]*M[1][1];
 
  F[0][0] = x;
  F[0][1] = y;
  F[1][0] = z;
  F[1][1] = w;
}
 
void power(double F[2][2], double n)
{
  double i;
  double M[2][2] = {{1,1},{1,0}};
 
  // n - 1 times multiply the matrix to {{1,0},{0,1}}
  for (i = 2; i <= n; i++)
      multiply(F, M);
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
