#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#ifdef NV_MALLOC
#include "tcnvmalloc.h"
#endif

#define BLKSIZE 256

pthread_mutex_t mtx;
int g_total_times = 8000;
int g_sum = 0;
int g_tdnum = 16;
double g_time_spent = 0;

void *twork(void *arg) {
    char *m;
    size_t i;
    clock_t begin, end;

    for (i=0; i<g_total_times/g_tdnum; i++) {
        pthread_mutex_lock(&mtx);
        if (g_sum == g_total_times) {
            pthread_mutex_unlock(&mtx);
            return NULL;
        }
        g_sum++;
        pthread_mutex_unlock(&mtx);

        begin = clock();
#ifdef NV_MALLOC
        m = (char *)nv_malloc(BLKSIZE);
#else
        m = (char *)malloc(BLKSIZE);
#endif
        end = clock();
        g_time_spent += (double)(end - begin);

        // m[0] = '0';
        // printf("%p\n", m);
#ifdef NV_MALLOC
        nv_free(m);
#else
        free(m);
#endif
    }

    return NULL;
}


int main(int argc, char *argv[]) {
    char *m;
    pthread_t tid[1000];
    int i, rc;

    if (argc >= 2) {
        g_total_times = atoi(argv[1]);
    }

    if (argc >= 3) {
        g_tdnum = atoi(argv[2]);
    }

    printf("total_times=%d, tdnum=%d\n", g_total_times, g_tdnum);
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

    printf("g_time_spent=%lf, avg clocks = %lf\n", g_time_spent, g_time_spent / (double)g_total_times);

    return 0;
}
