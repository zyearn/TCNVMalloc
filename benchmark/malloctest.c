#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#ifdef NV_MALLOC
#include "tcnvmalloc.h"
#endif

#define TDNUM 16
#define BLKSIZE 64
#define TOTALTIMES 8000

pthread_mutex_t mtx;
int g_sum = 0;

void *twork(void *arg) {
    char *m;
    size_t i;
    for (i=0; i<TOTALTIMES/TDNUM; i++) {
        pthread_mutex_lock(&mtx);
        if (g_sum == TOTALTIMES) {
            pthread_mutex_unlock(&mtx);
            return NULL;
        }
        g_sum++;
        pthread_mutex_unlock(&mtx);

#ifdef NV_MALLOC
        m = (char *)nv_malloc(BLKSIZE);
#else
        m = (char *)malloc(BLKSIZE);
#endif
        m[0] = '0';
        printf("%p\n", m);
#ifdef NV_MALLOC
        nv_free(m);
#else
        free(m);
#endif
    }

    return NULL;
}


int main() {
    char *m;
    pthread_t tid[TDNUM];
    int i;

    pthread_mutex_init(&mtx, NULL);

    for (i=0; i<TDNUM; i++) {
        if (pthread_create(&tid[i], NULL, &twork, NULL) < 0) {
            printf("pthread_create err\n");
        }
    }

    for (i=0; i<TDNUM; i++) {
        if (pthread_join(tid[i], NULL) < 0) {
            printf("pthread_join err\n");
        }
    }

    return 0;
}
