#include <time.h>
#include <stdio.h>
extern double fib(double n);
extern void MEMO_INIT();
extern void MEMO_SAVE();

int main() {
	MEMO_INIT();
	clock_t s, e;
	for (double i = 0; i <= 2048; i++) {
		s = clock();
		fib(i);
		e = clock();
		if ((int)i % 25 == 0)
		printf("(%d , %.14f) ",(int)i, ((double) e - s) / CLOCKS_PER_SEC);
	}
	MEMO_SAVE();
}
