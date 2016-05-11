#include "tcnvmalloc.h"
#include "pthread.h"

/* Global metadata */
pthread_once_t init_once = PTHREAD_ONCE_INIT;
thread_state_t global_state = UNINIT;
pthread_key_t destructor;
gpool_t gpool;

/* Mappings */
CACHE_ALIGN int cls2size[128];
char sizemap[256];
char sizemap2[128];

/* threads */
THREAD_LOCAL thread_state_t thread_state = UNINIT;
THREAD_LOCAL lheap_t *local_heap = NULL;

inline static void thread_init();
inline static void check_init();
inline static void global_init();
inline static void thread_exit();
static void gpool_init();
static void maps_init();
static void *small_malloc(int size_cls);
static void *large_malloc(size_t size);

/* global pool operation */
inline static void gpool_grow();
inline static chunkh_t *gpool_acquire_chunk();

/* helper */
inline static void *page_alloc(void *pos, size_t size);
inline static void page_free(void *pos, size_t size);
inline static lheap_t *acquire_lheap();
inline static void *chunk_alloc_obj(chunkh_t *ch);
inline static void chunk_init(chunkh_t *ch, int size_cls);
inline static chunkh_t *chunk_extract_header(void *ptr);
inline static void chunk_free_small(chunkh_t *ch, void *ptr);
inline static void chunk_free_large(void *ptr);
inline void lheap_replace_foreground(lheap_t *lh, int size_cls);

/* implementation */
static void gpool_init() {
    if (pthread_mutex_init(&gpool.lock, NULL) < 0) {
        fprintf(stderr, "fatal error: pthread_mutex_init failed\n");
        exit(-1);
    }

    void *ret = page_alloc(RAW_POOL_START, ALLOC_UNIT);
    if (ret < (int)0) {
        fprintf(stderr, "fatal error: page_alloc ailed\n");
        exit(-1);
    }

    gpool.pool_start = (void *)(((uint64_t)ret + CHUNK_SIZE - 1)/CHUNK_SIZE * CHUNK_SIZE);
    gpool.pool_end = ret + ALLOC_UNIT;
    gpool.free_start = gpool.pool_start;
}

static void maps_init() {
    int size;
    int class;

    for (size = 16, class = 0; size <= 256; size += 8, class++) {
        cls2size[class] = size;
    }

    for (size = 256; size < 65536; size <<= 1) {
        cls2size[class++] = size + (size >> 1);
        cls2size[class++] = size << 1;
    }

    int cur_class = 0;
    int cur_size = 0;

    for (cur_size = 8; cur_size <= 1024; cur_size += 8) {
        if (cur_size > cls2size[cur_class])
            cur_class++;
        sizemap[(cur_size - 1) >> 3] = cur_class;
    }
    
    /* init sizemap2 */
    for (cur_size = 1024; cur_size <= 65536; cur_size += 512) {
        if (cur_size > cls2size[cur_class])
            cur_class++;
        sizemap2[(cur_size - 1) >> 9] = cur_class;
    }
}

static void thread_exit() {
    //TODO: reclaim the memory space in local_heap
    free(local_heap);
}

static void global_init() {
    pthread_key_create(&destructor, thread_exit);
    gpool_init();
    maps_init();
    global_state = INITED;
}

static void check_init() {
    if (unlikely(thread_state != INITED)) {
        if (unlikely(global_state != INITED)) {
            pthread_once(&init_once, global_init);
        }
        thread_init();
    }
}

static void thread_init() {
    /* make thread_exit executed when thread quit */
    pthread_setspecific(destructor, (void *)1);

    local_heap = acquire_lheap();
    thread_state = INITED;
}

static void *chunk_alloc_obj(chunkh_t *ch) {
    void *ret = NULL;

    if (ch->free_mem_cnt > 0) {
        ret = ch->free_mem;
        ch->free_mem += ch->blk_size;
        ch->free_mem_cnt--;
    } else if (!dlist_empty(&ch->dlist_head, &ch->dlist_tail)) {
        ret = (void *)ch->dlist_tail.prev;
        dlist_remove((dlist_t *)ret);
    }

    return ret;
}

static void chunk_init(chunkh_t *ch, int size_cls) {
    ch->size_cls = size_cls;
    ch->blk_size = cls2size[size_cls];
    ch->blk_cnt = (CHUNK_DATA_SIZE) / ch->blk_size;
    ch->free_tot_cnt = ch->free_mem_cnt = ch->blk_cnt;
    ch->free_mem = (void *)ch + sizeof(chunkh_t);
    
    dlist_init(&ch->dlist_head, &ch->dlist_tail);
}

static chunkh_t *chunk_extract_header(void *ptr) {
    return (chunkh_t *)((uint64_t)ptr - (uint64_t)(ptr) % CHUNK_SIZE);
}

void lheap_replace_foreground(lheap_t *lh, int size_cls) {
    chunkh_t *ch;

    if (!list_empty(&(lh->background[size_cls]))) {
        ch = list_entry(lh->background[size_cls].next, chunkh_t, list);
        list_del(&ch->list);
        goto finish;
    }


    // TODO: allocate from free list

    /* get chunk from gpool */
    ch = gpool_acquire_chunk();
    check(ch != NULL, "gpool_acquire_chunk");
    ch->owner = lh;
    chunk_init(ch, size_cls);

finish:
    lh->foreground[size_cls] = ch;
    ch->state = FORG;
}

static void chunk_free_small(chunkh_t *ch, void *ptr) {
    // TODO: race condition
    
}

static void chunk_free_large(void *ptr) {
    // TODO

}

static void *small_malloc(int size_cls) {
    lheap_t *lh = local_heap;
    chunkh_t *ch;
    void *ret;
retry:
    ch = lh->foreground[size_cls];
    ret = chunk_alloc_obj(ch);

    if (unlikely(--ch->free_tot_cnt == 0)) {
        ch->state = FULL;
        lheap_replace_foreground(lh, size_cls);

        if (unlikely(ch->size_cls == DUMMY_CLASS)) {
            ch->free_tot_cnt = 1;
            goto retry;
        }
    }

    return ret;
}

static void *large_malloc(size_t size) {
    // TODO
    
    return NULL;
}
static void gpool_grow() {
    void *ret = page_alloc(gpool.pool_end, ALLOC_UNIT);
    if (ret < 0) {
        fprintf(stderr, "page_alloc failed\n");
        exit(-1);
    }

    gpool.pool_end += ALLOC_UNIT;
}

inline static chunkh_t *gpool_acquire_chunk() {
    // TODO: freelist
    pthread_mutex_lock(&gpool.lock);

    void *ret = gpool.free_start;
    gpool.free_start += CHUNK_SIZE;

    if (gpool.free_start > gpool.pool_end) {
        gpool_grow();
    }

    pthread_mutex_unlock(&gpool.lock);

    return (chunkh_t *)ret;
}

inline static int size2cls(size_t size) {
    int ret;
    if (likely(size <= 1024)) {
        ret = sizemap[(size - 1) >> 3];
    } else if (size <= 65536) {
        ret = sizemap2[(size - 1) >> 9];
    } else {
        ret = LARGE_CLASS;
    }

    return ret;
}

void *nv_malloc(size_t size) {
    void *ret = NULL;
    check_init();

    size += (size == 0);
    
    int size_cls = size2cls(size);
    if (likely(size_cls < DEFAULT_BLOCK_CLASS)) {
        ret = small_malloc(size_cls);
    } else if (size_cls == LARGE_CLASS) {
        ret = large_malloc(size);
    } else {
        fprintf(stderr, "fatal error: unknown class %d\n", size_cls);
        exit(0);
    }

    return ret;
}

void *nv_realloc(void *ptr, size_t size) {
    return NULL;
}

void nv_free(void *ptr) {
    if (ptr == NULL) {
        return;
    }

    chunkh_t *ch = chunk_extract_header(ptr);
    lheap_t *lh = local_heap;
    lheap_t *target_lh = ch->owner;

    if (likely(target_lh == lh) || likely((uint64_t)target_lh != LARGE_OWNER)) {
        chunk_free_small(ch, ptr);
    } else {
        chunk_free_large((void *)ch);
    }
}

static void *page_alloc(void *pos, size_t size) {
    return mmap(pos, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
}

static void page_free(void *pos, size_t size) {
    munmap(pos, size);
}

static lheap_t *acquire_lheap() {
    lheap_t *lh = (lheap_t *)malloc(sizeof(lheap_t));
    if (!lh) {
        fprintf(stderr, "in acquire_lheap: malloc failed\n");
        return NULL;
    }
    
    int i;
    for (i=0; i<DEFAULT_BLOCK_CLASS; i++) {
        lh->foreground[i] = &(lh->dummy_chunk);
        INIT_LIST_HEAD(&(lh->background[i]));
    }
    INIT_LIST_HEAD(&lh->free_head);

    lh->dummy_chunk.owner = lh;
    lh->dummy_chunk.size_cls = DUMMY_CLASS;
    lh->dummy_chunk.blk_size = 0;
    lh->dummy_chunk.blk_cnt = 0;
    lh->dummy_chunk.free_mem_cnt = 1;
    lh->dummy_chunk.free_tot_cnt = 1;
    lh->dummy_chunk.free_mem = (void *)0;
    dlist_init(&lh->dummy_chunk.dlist_head, &lh->dummy_chunk.dlist_tail);

    return lh;
}
