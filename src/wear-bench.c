#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "tcnvmalloc.h"

#define TDNUM 256
#define TIMES 100000
#define SIZE 128

void *twork(void *arg) {
    int i;
    void *m;

    for (i=0; i<TIMES; i++) {
        nv_malloc(SIZE);
        //printf("thread %zu: m=%p\n", (size_t)arg, m);
    }

    //printf("thread %zu ready to exit\n", (size_t)arg);
    return NULL;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "argc should > 2\n");
        return -1;
    }

    int n = atoi(argv[1]);
    if (n > 256) {
        fprintf(stderr, "too many threads\n");
        return -1;
    }

    int i;
    pthread_t tid[TDNUM];

    for (i=0; i<n; i++) {
        if (pthread_create(&tid[i], NULL, &twork, (void *)i) < 0) {
            printf("pthread_create err\n");
        }
    }

    void *m;
    m = nv_malloc(10);
    m = nv_malloc(10);

    for (i=0; i<n; i++) {
        if (pthread_join(tid[i], NULL) < 0) {
            printf("pthread_join err\n");
        }
    }

    return 0;
}
