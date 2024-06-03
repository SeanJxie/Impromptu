#include "tri.h"


struct TriList *TriList_create_empty() {
    struct TriList *ret = malloc(sizeof(struct TriList));
    ret->head = NULL;
    ret->len  = 0;

    return ret;
}

inline void TriList_insert(struct TriList *list, unsigned int index, struct Tri tri) {
    if (index > list->len) {
        return;
    }

    ++list->len;

    if (list->head == NULL) {
        list->head = malloc(sizeof(struct TriNode));
        list->head->next = NULL;
        list->head->tri = tri;

        return;
    }

    int i = 0;
    struct TriNode* one_before = list->head;
    while (i != index - 1) {
        one_before = one_before->next;
    }

    struct TriNode* inserted = malloc(sizeof(struct TriNode));
    inserted->next = one_before->next->next;
    inserted->tri = tri;

    one_before->next = inserted;
}

void TriList_destroy(struct TriList *list) {
    free(list);
}