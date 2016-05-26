#include "pq.h"

int pq_init(pq_t *pq, pq_comparator_pt comp, size_t size) {
    pq->pq = (void **)malloc(sizeof(void *) * (size+1));
    if (!pq->pq) {
        fprintf(stderr, "pq_init: malloc failed");
        exit(-1);
    }
    
    pq->nalloc = 0;
    pq->size = size + 1;
    pq->comp = comp;
    
    return 0;
}

int pq_is_empty(pq_t *pq) {
    return (pq->nalloc == 0)? 1: 0;
}

size_t pq_size(pq_t *pq) {
    return pq->nalloc;
}

void *pq_min(pq_t *pq) {
    if (pq_is_empty(pq)) {
        return NULL;
    }

    return pq->pq[1];
}

static int resize(pq_t *pq, size_t new_size) {
    if (new_size <= pq->nalloc) {
        fprintf(stderr, "resize: new_size to small");
        exit(-1);
    }

    void **new_ptr = (void **)malloc(sizeof(void *) * new_size);
    if (!new_ptr) {
        fprintf(stderr, "resize: malloc failed");
        exit(-1);
    }

    memcpy(new_ptr, pq->pq, sizeof(void *) * (pq->nalloc + 1));
    free(pq->pq);
    pq->pq = new_ptr;
    pq->size = new_size;
    return 0;
}

static void exch(pq_t *pq, size_t i, size_t j) {
    void *tmp = pq->pq[i];
    pq->pq[i] = pq->pq[j];
    pq->pq[j] = tmp;
}

static void swim(pq_t *pq, size_t k) {
    while (k > 1 && pq->comp(pq->pq[k], pq->pq[k/2])) {
        exch(pq, k, k/2);
        k /= 2;
    }
}

static size_t sink(pq_t *pq, size_t k) {
    size_t j;
    size_t nalloc = pq->nalloc;

    while (2*k <= nalloc) {
        j = 2*k;
        if (j < nalloc && pq->comp(pq->pq[j+1], pq->pq[j])) j++;
        if (!pq->comp(pq->pq[j], pq->pq[k])) break;
        exch(pq, j, k);
        k = j;
    }
    
    return k;
}

int pq_delmin(pq_t *pq) {
    if (pq_is_empty(pq)) {
        return 0;
    }

    exch(pq, 1, pq->nalloc);
    pq->nalloc--;
    sink(pq, 1);
    if (pq->nalloc > 0 && pq->nalloc <= (pq->size - 1)/4) {
        if (resize(pq, pq->size / 2) < 0) {
            return -1;
        }
    }

    return 0;
}

int pq_insert(pq_t *pq, void *item) {
    if (pq->nalloc + 1 == pq->size) {
        if (resize(pq, pq->size * 2) < 0) {
            return -1;
        }
    }

    pq->pq[++pq->nalloc] = item;
    swim(pq, pq->nalloc);

    return 0;
}

int pq_sink(pq_t *pq, size_t i) {
    return sink(pq, i);
}
