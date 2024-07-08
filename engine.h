#ifndef ENGINE_H
#define ENGINE_H

#include <stdio.h>
#include <string.h>

#include <SDL2/SDL.h>

#include "model.h"
#include "vector3.h"
#include "transform.h"
#include "util.h"
#include "light.h"

struct Engine {
    SDL_Window   *window;
    SDL_Renderer *renderer;

    // Window.
    int   window_width;
    int   window_height;
    int   num_window_pixels;
    float half_window_width;
    float half_window_height;
    float aspect_ratio;

    // Buffers.
    SDL_Texture   *frame_texture;
    unsigned char *color_buffer;
    float         *depth_buffer;
    size_t         color_buffer_size;
    size_t         depth_buffer_size;

    // Controls.
    float move_speed;
    float look_speed;

    // Render options.
    int wireframe;
    int backface_culling;
    int show_vertex_normals;
};

// We move Engine instances with heap pointers.

struct Engine *Engine_create(int window_width, int window_height);
void           Engine_destroy(struct Engine *e);

// Raster.
inline void    Engine_set_pixel(struct Engine *e, int x, int y, int r, int g, int b);
inline void    Engine_set_depth(struct Engine *e, int x, int y, float depth);
inline float   Engine_get_depth(struct Engine *e, int x, int y);
inline void    Engine_bresenham(struct Engine *e, int x1, int y1, int x2, int y2, int r, int g, int b);
inline void    Engine_raster_tri_wireframe(struct Engine *e, struct Vector3 v1, struct Vector3 v2, struct Vector3 v3, int r, int g, int b);

void           Engine_run(struct Engine *e);

#endif