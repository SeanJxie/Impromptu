#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include "engine.h"
#include "obj_parse.h"

int main(int argc, char *argv[]) {
    struct Engine *e = Engine_create(400 * 3, 300 * 3);

    Engine_run(e);
    Engine_destroy(e);

    // int len_string_list;
    // char string_list[MAX_NUM_SUBSTR][MAX_LEN_SUBSTR];
    // split("crazy happenings brev", string_list, &len_string_list);

    // for (int i = 0; i < len_string_list; ++i) {
    //     printf("%d: %s\n", i, string_list[i]);
    // }

    return EXIT_SUCCESS;
}