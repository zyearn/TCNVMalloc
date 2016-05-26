#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#ifdef WA_MALLOC
    #include "tcnvmalloc.h"
#else
    #ifdef NV_MALLOC
    #include "nvmalloc/nvmalloc.h"
    #endif
#endif

#define BLKSIZE 128

int g_total_times = 8000;
int g_sum = 0;
int g_tdnum = 16;
double g_time_spent = 0;

void *twork(void *arg) {
    char *m;
    size_t i;

    for (i=0; i<g_total_times; i++) {
#ifdef WA_MALLOC
        m = (char *)wa_malloc(BLKSIZE);
#else

#ifdef NV_MALLOC
        m = (char *)nvmalloc(BLKSIZE);
#else
        m = (char *)malloc(BLKSIZE);
#endif
#endif
    }

    return NULL;
}


int main(int argc, char *argv[]) {
    char *m;
    pthread_t tid[1000];
    int i, rc;
    srand(time(NULL));

    if (argc < 3) {
        fprintf(stderr, "usage: ./a.out <total_time> <threan_num>\n");
        exit(-1);
    }
    g_total_times = atoi(argv[1]);
    g_tdnum = atoi(argv[2]);

#ifdef NV_MALLOC
    nvmalloc_init(100000000, 10);
#endif
    //printf("total_times=%d, tdnum=%d\n", g_total_times, g_tdnum);

    struct timeval begin, end;
    gettimeofday(&begin, NULL);
    for (i=0; i<g_tdnum; i++) {
        if (pthread_create(&tid[i], NULL, &twork, NULL) < 0) {
            printf("pthread_create err\n");
        }
    }

    for (i=0; i<g_tdnum; i++) {
        if (pthread_join(tid[i], NULL) < 0) {
            printf("pthread_join err\n");
        }
    }
    gettimeofday(&end, NULL);

    //printf("g_time_spent=%lf, avg clocks = %lf\n", g_time_spent, g_time_spent / (double)g_total_times);
    int time_in_us = (end.tv_sec - begin.tv_sec) * 1000000 + (end.tv_usec - begin.tv_usec);
    printf("%lf", time_in_us / (double)(g_tdnum * g_total_times));


    return 0;
}
