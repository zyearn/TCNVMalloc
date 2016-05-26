#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PQ_DEFAULT_SIZE 200

typedef int (*pq_comparator_pt)(void *pi, void *pj);

typedef struct {
    void **pq;
    size_t nalloc;
    size_t size;
    pq_comparator_pt comp;
} pq_t;

inline int pq_init(pq_t *pq, pq_comparator_pt comp, size_t size);
inline int pq_is_empty(pq_t *pq);
inline size_t pq_size(pq_t *pq);
inline void *pq_min(pq_t *pq);
inline int pq_delmin(pq_t *pq);
inline int pq_insert(pq_t *pq, void *item);
inline int pq_sink(pq_t *pq, size_t i);

#endif 
