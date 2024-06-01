#ifndef ENGINE_H
#define ENGINE_H

#include <memory.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include "model.h"
#include "vector3.h"
#include "matrix4.h"

struct Engine {
    SDL_Window   *window;
    SDL_Renderer *renderer;

    int   window_width;
    int   window_height;
    float aspect_ratio;

    SDL_Texture   *frame_texture;
    unsigned char *color_buffer;
    float         *depth_buffer;
    size_t         color_buffer_size;
    size_t         depth_buffer_size;
};

// We move Engines instances with heap pointers.

struct Engine *Engine_create(int window_width, int window_height);
void           Engine_destroy(struct Engine *e);

// Raster.
inline void    Engine_set_pixel(struct Engine *e, int x, int y, int r, int g, int b);
inline void    Engine_bresenham(struct Engine *e, int x1, int y1, int x2, int y2, int r, int g, int b);
inline void    Engine_draw_tri_wireframe(struct Engine *e, const struct Tri t, int r, int g, int b);

void           Engine_run(struct Engine *e);

#endif