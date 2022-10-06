
double fib (double x);
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define MEMO_SIZE 2048
double MEMO_fib(double n){ if (n <= 1) { return n; } else { return fib(n - 1) + fib(n - 2); } }


        typedef struct MEMO_CACHE_fib_t {
            double n;
            double res1;
        } MEMO_CACHE_fib;

MEMO_CACHE_fib * MEMO_DATA_fib;
int MEMO_DONE_fib = 0;
int MEMO_LAST_USED_fib = 0;
double fib(double n) {
            double temp;
            if (!MEMO_DONE_fib) {
                temp = MEMO_fib(n);
                return temp;
            }
            else {
                for (int i = 0; i < MEMO_SIZE; i++) {
                   if (n == MEMO_DATA_fib[i].n) {
return MEMO_DATA_fib[i].res1;}
                }
                temp = MEMO_fib(n);
                if (MEMO_LAST_USED_fib >= MEMO_SIZE) {return temp;}
                MEMO_DATA_fib[MEMO_LAST_USED_fib].n=n;
                MEMO_DATA_fib[MEMO_LAST_USED_fib].res1 = temp;
                MEMO_LAST_USED_fib += 1;
                return temp;
            }
        }    
        void MEMO_INIT(void) { FILE*fp;
        MEMO_DATA_fib = malloc(MEMO_SIZE * sizeof(MEMO_CACHE_fib));
        if (!MEMO_DATA_fib) {
            printf("malloc failed\n");
            exit(1);
        }
        

        if (access("MEMO_CACHE_fib", F_OK) == 0) {
            fp = fopen("MEMO_CACHE_fib", "rb");
            int temp = fread(MEMO_DATA_fib,MEMO_SIZE * sizeof(MEMO_CACHE_fib),1,fp);
            if (temp == EOF) {
                printf("fread failed\n");
                printf("%d\n", ferror(fp));
                perror("ERROR: ");
                exit(1);
            }
        } else {
            for (int i = 0; i < MEMO_SIZE; i++) {
                MEMO_DATA_fib[i].n=NAN;
                MEMO_DATA_fib[i].res1 = NAN;
            }
        }
        MEMO_DONE_fib = 1;
    }void MEMO_SAVE(void) { FILE*fp;
        fp = fopen("MEMO_CACHE_fib", "wb");
        fwrite(MEMO_DATA_fib,MEMO_SIZE * sizeof(MEMO_DATA_fib),1,fp);
    }