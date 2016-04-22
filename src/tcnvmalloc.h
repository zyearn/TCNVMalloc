#ifndef TCNVMALLOC_H_
#define TCNVMALLOC_H_

#include <stdlib.h>

#define THREAD_LOCAL __attribute__ ((tls_model ("initial-exec"))) __thread


void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);

#endif
