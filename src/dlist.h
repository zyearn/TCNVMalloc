#ifndef DLIST_H_
#define DLIST_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dlist_s dlist_t;

struct dlist_s {
    dlist_t *next, *prev;
};

static inline void dlist_init(dlist_t *head, dlist_t *tail) {
    if (head == NULL || tail == NULL) {
        return; 
    }

    head->prev = NULL;
    head->next = tail;
    tail->prev = head;
    tail->next = NULL;
}

static inline int dlist_empty(dlist_t *head, dlist_t *tail) {
    return (head->next == tail && tail->prev == head);
}

static inline void dlist_remove(dlist_t *elem) {
    elem->prev->next = elem->next;
    elem->next->prev = elem->prev;
}

static inline void dlist_add(dlist_t* head, dlist_t *elem) {
    dlist_t *next = head->next;

    head->next = elem;
    elem->prev = head;
    elem->next = next;
    next->prev = elem;
}

#ifdef __cplusplus
}
#endif

#endif 

