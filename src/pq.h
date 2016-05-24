#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ZV_PQ_DEFAULT_SIZE 200

typedef int (*zv_pq_comparator_pt)(void *pi, void *pj);

typedef struct {
    void **pq;
    size_t nalloc;
    size_t size;
    zv_pq_comparator_pt comp;
} zv_pq_t;

int zv_pq_init(zv_pq_t *zv_pq, zv_pq_comparator_pt comp, size_t size);
int zv_pq_is_empty(zv_pq_t *zv_pq);
size_t zv_pq_size(zv_pq_t *zv_pq);
void *zv_pq_min(zv_pq_t *zv_pq);
int zv_pq_delmin(zv_pq_t *zv_pq);
int zv_pq_insert(zv_pq_t *zv_pq, void *item);

int zv_pq_sink(zv_pq_t *zv_pq, size_t i);

#endif 
