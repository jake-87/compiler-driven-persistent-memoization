
#ifndef ___MEMO_STDLIB
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
#define ___MEMO_LEN 1
#define ___MEMO_STDLIB
#endif

typedef struct ___MEMO_diff_T {
  double x;
  double y;

  long _hits;
  double _val;
} ___MEMO_diff_T;

double ___MEMO_diff(double x, double y) {
  if (x > y) {
    return x - y;
  }
  return y - x;
}

___MEMO_diff_T *___MEMO_diff_cache = NULL;

double diff(double x, double y) {
  if (___MEMO_diff_cache == NULL) {
    ___MEMO_diff_cache =
        mmap(0, ___MEMO_LEN * sizeof(*___MEMO_diff_cache),
             PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if (!___MEMO_diff_cache) {
      printf("alloc fail\n");
      exit(1);
    }

    memset(___MEMO_diff_cache, -1, ___MEMO_LEN * sizeof(*___MEMO_diff_cache));
  }

  if (___MEMO_diff_cache[0]._hits == -1) {
    if (fork() == 0) {
      // loading must be done :despair:
      ___MEMO_diff_cache[0]._hits = 0;
      if (access("___MEMO_diff_cache", F_OK) == 0) {
        FILE *fp = fopen("___MEMO_diff_cache", "r");

        fread(___MEMO_diff_cache, ___MEMO_LEN * sizeof(*___MEMO_diff_cache), 1,
              fp);
        fclose(fp);
      } else {
        memset(___MEMO_diff_cache, 0,
               ___MEMO_LEN * sizeof(*___MEMO_diff_cache));
      }
      exit(0);
    }
    else {
      usleep(2);
    }
  }

  if (___MEMO_diff_cache[0]._hits == -1) {
    return ___MEMO_diff(x, y);
  }

  for (int i = 0; i < ___MEMO_LEN; i++) {
    if (___MEMO_diff_cache[i].x == x) {
      if (___MEMO_diff_cache[i].y == y) {
        ___MEMO_diff_cache[i]._hits += 1;
        return ___MEMO_diff_cache[i]._val;
      }
    }
  }

  double temp = ___MEMO_diff(x, y);
  for (int i = 0; i < ___MEMO_LEN; i++) {
    if (___MEMO_diff_cache[i]._hits == 0) {
      ___MEMO_diff_cache[i].x = x;
      ___MEMO_diff_cache[i].y = y;
      ___MEMO_diff_cache[i]._hits += 1;
      ___MEMO_diff_cache[i]._val = temp;
      return temp;
    }
  }
}

void ___MEMO_diff_save(void) {
  FILE *fp = fopen("___MEMO_diff_cache", "w");
  fwrite(___MEMO_diff_cache, ___MEMO_LEN * sizeof(*___MEMO_diff_cache), 1, fp);
  fclose(fp);
  munmap(___MEMO_diff_cache, ___MEMO_LEN * sizeof(*___MEMO_diff_cache));
}

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

typedef struct ___MEMO_mysqrt_T {
  double n;

  long _hits;
  double _val;
} ___MEMO_mysqrt_T;

double ___MEMO_mysqrt(double n) {
  double i = 1, count = n;
  usleep(((long)n % 64) * 10);
  while (diff(i * i, n) > 0.001) {
    if (i * i > n) {
      i -= count * 0.5;
    } else {
      i += count * 0.5;
    }
    count = count / 2;
    if (count == 0) {
      if (diff(i * i, n) < 0.5) {
        return i;
      }
      count = 1;
    }
  }
  return i;
}

___MEMO_mysqrt_T *___MEMO_mysqrt_cache = NULL;

double mysqrt(double n) {
  if (___MEMO_mysqrt_cache == NULL) {
    ___MEMO_mysqrt_cache =
        mmap(0, ___MEMO_LEN * sizeof(*___MEMO_mysqrt_cache),
             PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if (!___MEMO_mysqrt_cache) {
      printf("alloc fail\n");
      exit(1);
    }

    memset(___MEMO_mysqrt_cache, -1,
           ___MEMO_LEN * sizeof(*___MEMO_mysqrt_cache));
  }

  if (___MEMO_mysqrt_cache[0]._hits == -1) {
    if (fork() == 0) {
      // loading must be done :despair:
      ___MEMO_mysqrt_cache[0]._hits = 0;
      if (access("___MEMO_mysqrt_cache", F_OK) == 0) {
        FILE *fp = fopen("___MEMO_mysqrt_cache", "r");

        fread(___MEMO_mysqrt_cache, ___MEMO_LEN * sizeof(*___MEMO_mysqrt_cache),
              1, fp);
        fclose(fp);
      } else {
        memset(___MEMO_mysqrt_cache, 0,
               ___MEMO_LEN * sizeof(*___MEMO_mysqrt_cache));
      }
      exit(0);
    }
  }

  if (___MEMO_mysqrt_cache[0]._hits == -1) {
    return ___MEMO_mysqrt(n);
  }

  for (int i = 0; i < ___MEMO_LEN; i++) {
    if (___MEMO_mysqrt_cache[i].n == n) {
      ___MEMO_mysqrt_cache[i]._hits += 1;
      return ___MEMO_mysqrt_cache[i]._val;
    }
  }

  double temp = ___MEMO_mysqrt(n);
  for (int i = 0; i < ___MEMO_LEN; i++) {
    if (___MEMO_mysqrt_cache[i]._hits == 0) {
      ___MEMO_mysqrt_cache[i].n = n;
      ___MEMO_mysqrt_cache[i]._hits += 1;
      ___MEMO_mysqrt_cache[i]._val = temp;
      return temp;
    }
  }
}

void ___MEMO_mysqrt_save(void) {
  FILE *fp = fopen("___MEMO_mysqrt_cache", "w");
  fwrite(___MEMO_mysqrt_cache, ___MEMO_LEN * sizeof(*___MEMO_mysqrt_cache), 1,
         fp);
  fclose(fp);
  munmap(___MEMO_mysqrt_cache, ___MEMO_LEN * sizeof(*___MEMO_mysqrt_cache));
}
