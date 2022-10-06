
#ifndef ___MEMO_STDLIB
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
#define ___MEMO_LEN 1024
#define ___MEMO_STDLIB
#endif

double fib(double);

typedef struct ___MEMO_fib_T {
  double n;

  long _hits;
  double _val;
} ___MEMO_fib_T;

double ___MEMO_fib(double n) {
  if (n <= 1) {
    return n;
  } else {
    return fib(n - 1) + fib(n - 2);
  }
}

___MEMO_fib_T *___MEMO_fib_cache = NULL;

double fib(double n) {
  if (___MEMO_fib_cache == NULL) {
    ___MEMO_fib_cache =
        mmap(0, ___MEMO_LEN * sizeof(*___MEMO_fib_cache),
             PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if (!___MEMO_fib_cache) {
      printf("alloc fail\n");
      exit(1);
    }

    memset(___MEMO_fib_cache, -1, ___MEMO_LEN * sizeof(*___MEMO_fib_cache));
  }

  if (___MEMO_fib_cache[0]._hits == -1) {
    if (fork() == 0) {
      // loading must be done :despair:
      ___MEMO_fib_cache[0]._hits = 0;
      if (access("___MEMO_fib_cache", F_OK) == 0) {
        FILE *fp = fopen("___MEMO_fib_cache", "r");
        if (!fp) {
          // Rational: If we can't open the file, pretend it doesn't exist
          goto cantaccess;
        }
        fread(___MEMO_fib_cache, ___MEMO_LEN * sizeof(*___MEMO_fib_cache), 1,
              fp);
        fclose(fp);
      } else {
      cantaccess:
        memset(___MEMO_fib_cache, 0, ___MEMO_LEN * sizeof(*___MEMO_fib_cache));
      }
      exit(0);
    } else {
      usleep(2);
    }
  }

  if (___MEMO_fib_cache[0]._hits == -1) {
    return ___MEMO_fib(n);
  }

  for (int i = 0; i < ___MEMO_LEN; i++) {
    if (___MEMO_fib_cache[i].n == n) {
      ___MEMO_fib_cache[i]._hits += 1;
      return ___MEMO_fib_cache[i]._val;
    }
  }

  double temp = ___MEMO_fib(n);
  for (int i = 0; i < ___MEMO_LEN; i++) {
    if (___MEMO_fib_cache[i]._hits == 0) {
      ___MEMO_fib_cache[i].n = n;
      ___MEMO_fib_cache[i]._hits += 1;
      ___MEMO_fib_cache[i]._val = temp;
      return temp;
    }
  }
}

void ___MEMO_fib_save(void) {
  FILE *fp = fopen("___MEMO_fib_cache", "w");
  fwrite(___MEMO_fib_cache, ___MEMO_LEN * sizeof(*___MEMO_fib_cache), 1, fp);
  fclose(fp);
  munmap(___MEMO_fib_cache, ___MEMO_LEN * sizeof(*___MEMO_fib_cache));
}
