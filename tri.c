#include "tri.h"

// struct Tri Tri_create(struct Vector3 p1, struct Vector3 p2, struct Vector3 p3, unsigned char r, unsigned char g, unsigned char b) {
//     struct Vector3 u = Vector3_sub(p2, p1);
//     struct Vector3 v = Vector3_sub(p3, p1);
//     struct Vector3 n = Vector3_normalize(Vector3_cross(u, v));

//     return (struct Tri){
//         p1, p2, p3,
//         n,
//         r, g, b
//     };
// }

// struct TriList *TriList_create_empty() {
//     struct TriList *ret = malloc(sizeof(struct TriList));
//     ret->head = NULL;
//     ret->len  = 0;

//     return ret;
// }

// inline void TriList_insert(struct TriList *list, unsigned int index, struct Tri tri) {
//     if (index > list->len) {
//         return;
//     }

//     ++list->len;

//     if (list->head == NULL) {
//         list->head = malloc(sizeof(struct TriNode));
//         list->head->next = NULL;
//         list->head->tri = tri;

//         return;
//     }

//     int i = 0;
//     struct TriNode* one_before = list->head;
//     while (i != index - 1) {
//         one_before = one_before->next;
//     }

//     struct TriNode* inserted = malloc(sizeof(struct TriNode));
//     inserted->next = one_before->next->next;
//     inserted->tri = tri;

//     one_before->next = inserted;
// }

// void TriList_destroy(struct TriList *list) {
//     free(list);
// }