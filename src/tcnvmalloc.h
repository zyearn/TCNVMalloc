#ifndef TCNVMALLOC_H_
#define TCNVMALLOC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/mman.h>
#include "list.h"
#include "dbg.h"

#define THREAD_LOCAL __attribute__ ((tls_model ("initial-exec"))) __thread
#define likely(x)           __builtin_expect(!!(x),1)
#define unlikely(x)         __builtin_expect(!!(x),0)
#define CACHE_LINE_SIZE     64
#define CACHE_ALIGN __attribute__ ((aligned (CACHE_LINE_SIZE)))
#define DEFAULT_BLOCK_CLASS (100)
#define LARGE_CLASS         (100)
#define DUMMY_CLASS         (101)
#define LARGE_OWNER         (0x5AA5)

/* configuration */
#define PAGE_SIZE 4096
#define CHUNK_DATA_SIZE (16*PAGE_SIZE)
#define CHUNK_SIZE (CHUNK_DATA_SIZE + sizeof(chunkh_t))
#define RAW_POOL_START      ((void*)((0x600000000000/CHUNK_SIZE+1)*CHUNK_SIZE))
#define ALLOC_UNIT  (1024*1024*1024)


typedef struct gpool_s gpool_t;
typedef struct lheap_s lheap_t;
typedef struct chunkh_s chunkh_t;

typedef enum {
    UNINIT,
    INITED
} thread_state_t;

typedef enum {
    FORG,
    BACK,
    FULL,
} chunk_state_t;

struct chunkh_s {
    lheap_t *owner;
    chunk_state_t state;
    uint32_t size_cls;
    uint32_t blk_size;
    uint32_t blk_cnt;
    uint32_t free_blk_cnt;
    void *free_mem;

    list_head list;
};

struct gpool_s {
    pthread_mutex_t lock;
    void *pool_start;
    void *pool_end;
    void *free_start;
};

struct lheap_s {
    chunkh_t *foreground[DEFAULT_BLOCK_CLASS];
    list_head background[DEFAULT_BLOCK_CLASS];
    list_head free_head;

    chunkh_t dummy_chunk;
};


void *nv_malloc(size_t size);
void *nv_realloc(void *ptr, size_t size);
void nv_free(void *ptr);

#ifdef __cplusplus
}
#endif

#endif
