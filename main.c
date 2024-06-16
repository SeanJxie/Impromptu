#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include "engine.h"

int main(int argc, char *argv[]) {
    struct Engine *e = Engine_create(400 * 3, 300 * 3);

    Engine_run(e);
    Engine_destroy(e);

    // struct Matrix4 a;

    // a.x00 = 1;
    // a.x01 = 2;
    // a.x02 = 3;
    // a.x03 = 4;

    // a.x10 = 2;
    // a.x11 = 2;
    // a.x12 = 2;
    // a.x13 = 2;

    // a.x20 = 57;
    // a.x21 = 2;
    // a.x22 = 5;
    // a.x23 = 0;

    // a.x30 = 9;
    // a.x31 = 9;
    // a.x32 = 9;
    // a.x33 = 7;

    // Matrix4_print(&a);
    // printf("\n");
    // Matrix4_transpose(&a, &a);
    // Matrix4_print(&a);
    // printf("\n");

    return EXIT_SUCCESS;
}