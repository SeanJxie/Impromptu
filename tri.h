#ifndef TRI_H
#define TRI_H

#include <stdlib.h>

#include "vector3.h"

struct Tri {
    struct Vector3 p1, p2, p3;
    struct Vector3 normal;
    unsigned char r, g, b;
};

struct Tri Tri_create(struct Vector3 p1, struct Vector3 p2, struct Vector3 p3, unsigned char r, unsigned char g, unsigned char b);

// We move Tri instances by value.

// struct TriNode {
//     struct Tri tri;
//     struct TriNode *next;
// };

// // We move TriNode instance by heap pointer.

// struct TriList {
//     struct TriNode *head;
//     int             len;
// };

// // We move TriList instances by heap pointer.

// struct TriList *TriList_create_empty();
// struct TriList TriList_create_from_array(struct Tri *tri_array);

// inline void TriList_insert(struct TriList *list, int index, struct Tri tri);

// void TriList_destroy(struct TriList *list);

#endif