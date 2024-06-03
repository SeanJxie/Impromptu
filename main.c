#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include "engine.h"

int main(int argc, char *argv[]) {
    struct Engine *e = Engine_create(1280, 960);

    Engine_run(e);
    Engine_destroy(e);

    return EXIT_SUCCESS;
}