#include "tcnvmalloc.h"

/* Global metadata */

THREAD_LOCAL int n = 0;
THREAD_LOCAL int thread_state = 0;

static void thread_init();
static void check_init();

void check_init() {
    if (thread_state == 0) {
        thread_init();
    }
}

void thread_init() {
    thread_state = 1;
}

void *malloc(size_t size) {
    check_init();

    return (void *)n++;
}

void *realloc(void *ptr, size_t size) {
    return NULL;
}

void free(void *ptr) {
    return;
}
