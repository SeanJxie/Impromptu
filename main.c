#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include "engine.h"
#include "obj_parse.h"

int main(int argc, char *argv[]) {
    struct Engine *e = Engine_create(3840 / 2, 2160 / 2);

    Engine_run(e);
    Engine_destroy(e);

    return EXIT_SUCCESS;
}