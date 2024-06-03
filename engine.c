#include "engine.h"

/*

TODO: Clipping
      Backface culling
      Depth check (filling and using depth buffer)
      Put raster function in their own raster.c/.h

      Test Model-View-Projection (MVP) matrix performance.
*/

struct Engine *Engine_create(int window_width, int window_height) {
    printf("Engine_create: Initializing engine.\n");

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"); // Linear scaling interpolation.

    struct Engine *e = malloc(sizeof(struct Engine));
    e->window_width  = window_width;
    e->window_height = window_height;
    e->aspect_ratio  = (float)window_width / window_height;

    // Window and renderer.
    e->window = SDL_CreateWindow(
        "Impromptu", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        e->window_width, e->window_height, 
        SDL_WINDOW_SHOWN
    );
    e->renderer = SDL_CreateRenderer(e->window, -1, SDL_RENDERER_ACCELERATED);
    
    //SDL_RenderSetLogicalSize(e->renderer, 100, 100);

    // Frame data.
    e->frame_texture = SDL_CreateTexture(
        e->renderer, 
        SDL_PIXELFORMAT_ABGR8888,    // 4 bytes (each 8 bit) representing alpha, blue, green, and red for each pixel.
        SDL_TEXTUREACCESS_STREAMING, // Changes frequently, lockable.
        e->window_width,
        e->window_height
    );

    // Initialize frame buffer (comprised of a color and depth buffer in this case).

    e->color_buffer_size = sizeof(unsigned char) * window_width * window_height * 4;
    e->depth_buffer_size = sizeof(float) * window_width * window_height;

    e->color_buffer = malloc(e->color_buffer_size);
    e->depth_buffer = malloc(e->depth_buffer_size);

    memset(e->color_buffer, 0, e->color_buffer_size);
    memset(e->depth_buffer, 0, e->depth_buffer_size);

    return e;
}

void Engine_destroy(struct Engine *e) {
    printf("Engine_destroy: Destroying engine.\n");

    SDL_DestroyTexture(e->frame_texture);
    SDL_DestroyRenderer(e->renderer);
    SDL_DestroyWindow(e->window);
    SDL_Quit();

    free(e->color_buffer);
    free(e->depth_buffer);
    free(e);
}

inline void Engine_set_pixel(struct Engine *e, int x, int y, int r, int g, int b) {
    // Ensure a valid color buffer index.
    if (0 <= x && x < e->window_width && 0 <= y && y < e->window_height) {
        int color_offset = (e->window_width * y * 4) + x * 4;

        e->color_buffer[color_offset + 0] = r;
        e->color_buffer[color_offset + 1] = g;
        e->color_buffer[color_offset + 2] = b;
        e->color_buffer[color_offset + 3] = SDL_ALPHA_OPAQUE;
    }
}

inline void Engine_bresenham(struct Engine *e, int x1, int y1, int x2, int y2, int r, int g, int b) {
    int steep = abs(y2 - y1) > abs(x2 - x1);
    int inc = -1;
    int tmp;

    if (steep) {
        tmp = x1;
        x1 = y1;
        y1 = tmp;

        tmp = x2;
        x2 = y2;
        y2 = tmp;
    }

    if (x1 > x2) {
        tmp = x1;
        x1 = x2;
        x2 = tmp;

        tmp = y1;
        y1 = y2;
        y2 = tmp;
    }

    if (y1 < y2) {
        inc = 1;
    }

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int y = y1;
    int x = x1;
    int e_ = 0;

    while (x <= x2) {
        if (steep) {
            Engine_set_pixel(e, y, x, r, g, b);
        } else {
            Engine_set_pixel(e, x, y, r, g, b);
        }

        if ((e_ + dy) << 1 < dx) {
            e_ = e_ + dy;
        } else {
            y += inc;
            e_ = e_ + dy - dx;
        }
        
        ++x;
    }
}

inline void Engine_draw_tri_wireframe(struct Engine *e, struct Vector3 v1, struct Vector3 v2, struct Vector3 v3, int r, int g, int b) {
    Engine_bresenham(e, v1.x, v1.y, v2.x, v2.y, r, g, b);
    Engine_bresenham(e, v2.x, v2.y, v3.x, v3.y, r, g, b);
    Engine_bresenham(e, v3.x, v3.y, v1.x, v1.y, r, g, b);
}


// Outputs the model in clip space.
inline void Engine_mvp(const struct Tri *mesh, int num_tris, const struct Matrix4 *mvp, struct Tri *out) {
    // Mesh comes in model-local coordiantes.
    struct Tri tri;

    for (int i = 0; i < num_tris; ++i) {
        tri = mesh[i];
        out[i].p1 = Vector3_mul_Matrix4(tri.p1, mvp);
        out[i].p2 = Vector3_mul_Matrix4(tri.p2, mvp);
        out[i].p3 = Vector3_mul_Matrix4(tri.p3, mvp);
    }
}    

void Engine_run(struct Engine *e) {
    printf("Engine_run: running engine.\n");

    // In general, we use left-handed coordinate systems.
    // That is, +x is leftwards.
    // Also,    +y is downwards.
    // Also,    +z is forwards.

    // Models.
    struct Model *model = Model_from_obj(
        "models/cow.obj", 
        0, 0, 10, 
        180, 0, 0, 
        1, 1, 1
    );

    // Transformations.
    struct Matrix4 projection;
    Matrix4_perspective(90, e->aspect_ratio, 2, 20, &projection);

    struct Matrix4 view;
    struct Vector3 camera_pos = Vector3_create_point(0, 0, 0);
    Matrix4_look_at(
        camera_pos, 
        Vector3_create_point(0, 0, camera_pos.z + 1),
        Vector3_create_direction(0, 1, 0), 
        &view
    );

    struct Matrix4 model_view;
    struct Matrix4 model_view_projection;

    struct Matrix4 viewport;
    Matrix4_viewport(e->window_width, e->window_height, &viewport);

    // Intermediate Tri buffers.

    // Tris in here will be in clip space. Same number of Tris as the model.
    struct Tri *clip_space_mesh = malloc(sizeof(struct Tri) * model->num_tris);

    // Tris in here will be in clip space. Some the number of Tris will change depending on
    // culling and clipping. This array is dynamic.
    //struct Tri *clipped_mesh = malloc(sizeof(struct Tri) * model->num_tris);

    // Tris in here will undergo perspective divide and finally rasterization.
    //struct Tri *raster_mesh = malloc(sizeof(struct Tri) * model->num_tris);

    // Timing.
    Uint64 frame_start = 0;
    Uint64 frame_end   = SDL_GetPerformanceCounter();
    float dt = 0;
    char fps_string[20];
    
    int running = 1;
    SDL_Event event;

    while (running) {
        frame_start = frame_end;
        frame_end   = SDL_GetPerformanceCounter();
        dt = (float)((frame_end - frame_start) * 1000 / (float)SDL_GetPerformanceFrequency());
        sprintf(fps_string, "Impromptu | FPS: %d", (int)(1000.0 / dt));
        SDL_SetWindowTitle(e->window, fps_string);

        // Handle user input events.
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_w) {
                    Model_translate(model, 0, 0, 0.1);
                } else if (event.key.keysym.sym == SDLK_s) {
                    Model_translate(model, 0, 0, -0.1);
                } else if (event.key.keysym.sym == SDLK_d) {
                    Model_translate(model, 0.1, 0, 0);
                } else if (event.key.keysym.sym == SDLK_a) {
                    Model_translate(model, -0.1, 0, 0);
                } else if (event.key.keysym.sym == SDLK_LSHIFT) {
                    Model_translate(model, 0, 0.1, 0);
                } else if (event.key.keysym.sym == SDLK_SPACE) {
                    Model_translate(model, 0, -0.1, 0);
                }
            }
        }

        // Model manipulation.
        Model_rotate(model, 0, 0.05 * dt, 0);

        // Clear frame.
        memset(e->color_buffer, 20, e->color_buffer_size);
        
        // Construct and apply the mvp matrix.
        Matrix4_mul(&view, &model->model_to_world, &model_view);
        Matrix4_mul(&projection, &model_view, &model_view_projection);

        Engine_mvp(model->mesh, model->num_tris, &model_view_projection, clip_space_mesh);

        // Draw the object.
        for (int i = 0; i < model->num_tris; ++i) {
            struct Tri t = clip_space_mesh[i]; // t is a copy.
            struct Vector3 vert1 = t.p1;
            struct Vector3 vert2 = t.p2;
            struct Vector3 vert3 = t.p3;
            
            // // --- MODEL-LOCAL SPACE ----

            // // Apply the model matrix to convert the local Tri vertices to world coordinates.
            // vert1 = Vector3_mul_Matrix4(vert1, &model->model_to_world);
            // vert2 = Vector3_mul_Matrix4(vert2, &model->model_to_world);
            // vert3 = Vector3_mul_Matrix4(vert3, &model->model_to_world);

            // // --- WORLD SPACE ----

            // // Compute and apply view matrix.
            // // TODO: recompute only when view has changed.
            

            // vert1 = Vector3_mul_Matrix4(vert1, &view);
            // vert2 = Vector3_mul_Matrix4(vert2, &view);
            // vert3 = Vector3_mul_Matrix4(vert3, &view);

            // // --- CAMERA SPACE ----

            // // Apply perspective projection matrix.
            // vert1 = Vector3_mul_Matrix4(vert1, &persp);
            // vert2 = Vector3_mul_Matrix4(vert2, &persp);
            // vert3 = Vector3_mul_Matrix4(vert3, &persp);

            // --- HOMOGENEOUS CLIP SPACE ----            
            //
            // Here, each vertex (x, y, z, w) satisfies
            //
            //   -w < x < w, -w < y < w, 0 < z < w
            //
            // if and only if the vertex lies within the viewing
            // frustrum.
            //
            // We can cull and clip triangles based on this property
            // to ease the load on the upcoming perspective division  
            // and rasterization stages.

            // -- CULL --
            // 
            // If the entire triangle is outside the view frustrum, don't even bother with
            // perspective divide and rasterization.

            // Entire triangle is out left.
            if (vert1.x < -vert1.w && 
                vert2.x < -vert2.w && 
                vert3.x < -vert3.w) {
                continue;
            }

            // Entire triangle is out right.
            if (vert1.x > vert1.w && 
                vert2.x > vert2.w && 
                vert3.x > vert3.w) {
                continue;
            }

            // Entire triangle is out top.
            if (vert1.y < -vert1.w && 
                vert2.y < -vert2.w && 
                vert3.y < -vert3.w) {
                continue;
            }

            // Entire triangle is out bottom.
            if (vert1.y > vert1.w && 
                vert2.y > vert2.w && 
                vert3.y > vert3.w) {
                continue;
            }

            // Entire triangle is out near.
            if (vert1.z < 0 && 
                vert2.z < 0 && 
                vert3.z < 0) {
                continue;
            }

            // Entire triangle is out far.
            if (vert1.z > vert1.w && 
                vert2.z > vert2.w && 
                vert3.z > vert3.w) {
                continue;
            }

            // -- CLIP --
            //
            // If the triangle is partially outside the view frustrum, we would still like
            // to proceed with perspective divide and rasterization, but only after removing
            // the outside vertices and reconstructing the triangle as one or more triangles.
            // 
            // Most important: clip against near plane to avoid division by zero since w is mapped to 
            // camera space z in the perspective matrix. 
            //
            // Under no circumstances do we want a vertex zero or very close to zero. So, on top of the culling, 
            // we need to do clipping.

            // One vertex out near.

            // if ((vert1.z < 0 && vert2.z >= 0 && vert3.z >= 0) || 
            //     (vert2.z < 0 && vert1.z >= 0 && vert3.z >= 0) ||
            //     (vert3.z < 0 && vert1.z >= 0 && vert2.z >= 0)) {

                
            // }

            vert1 = Vector3_smul(vert1, 1 / vert1.w);
            vert2 = Vector3_smul(vert2, 1 / vert2.w);
            vert3 = Vector3_smul(vert3, 1 / vert3.w);

            // --- NDC (Normalized Device Coordinate) SPACE ----

            // Apply viewport transform to obtain screen coordinates.
            vert1 = Vector3_mul_Matrix4(vert1, &viewport);
            vert2 = Vector3_mul_Matrix4(vert2, &viewport);
            vert3 = Vector3_mul_Matrix4(vert3, &viewport);

            // --- SCREEN SPACE ----

            // Perform rasterization of triangle.
            Engine_draw_tri_wireframe(e, vert1, vert2, vert3, 255, 250, 250);
        }

        // Copy pixels to texture.
        // SDL_UpdateTexture(e->frame_texture, NULL, e->frame_pixels, e->window_width * 4);
        unsigned char *locked_pixels;
        int pitch; // Dummy variable.
        SDL_LockTexture(e->frame_texture, NULL, (void**)&locked_pixels, &pitch);
        memcpy(locked_pixels, e->color_buffer, e->color_buffer_size);
        SDL_UnlockTexture(e->frame_texture);

        // Copy texture to renderer.
        SDL_RenderCopy(e->renderer, e->frame_texture, NULL, NULL);
        
        // Present renderer.
        SDL_RenderPresent(e->renderer);
    }

    // TEMPORARY.
    Model_destroy(model);
    free(clip_space_mesh);
}
