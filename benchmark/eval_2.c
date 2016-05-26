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

#define BLKSIZE 32

pthread_mutex_t mtx;
int g_total_times = 8000;
int g_sum = 0;
int g_tdnum = 16;
double g_time_spent = 0;

void *twork(void *arg) {
    char *m;
    size_t i;
    clock_t begin, end;

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
        m[0] = '\0';

        if (rand() % 2 == 0) {
#ifdef WA_MALLOC
        wa_free(m);
#else
#ifdef NV_MALLOC
        nvfree(m);
#else
        free(m);
#endif
#endif
        }
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

    rc = pthread_mutex_init(&mtx, NULL);
    if (rc < 0) {
        fprintf(stderr, "pthread_mutex_init error\n");
        exit(-1);
    }

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

    //printf("g_time_spent=%lf, avg clocks = %lf\n", g_time_spent, g_time_spent / (double)g_total_times);
    
    return 0;
}
