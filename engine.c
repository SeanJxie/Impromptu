#include "engine.h"

/*

TODO: Clipping
      Backface culling
      Depth check (filling and using depth buffer)
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
    int offset = (e->window_width * y * 4) + x * 4;
    e->color_buffer[offset + 0] = r;
    e->color_buffer[offset + 1] = g;
    e->color_buffer[offset + 2] = b;
    e->color_buffer[offset + 3] = SDL_ALPHA_OPAQUE;
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

    for (; x <= x2; ++x) {
        if (steep) {
            Engine_set_pixel(e, y, x, r, g, b);
        }
        else {
            Engine_set_pixel(e, x, y, r, g, b);
        }

        if ((e_ + dy) << 1 < dx) {
            e_ = e_ + dy;
        }
        else {
            y += inc;
            e_ = e_ + dy - dx;
        }
    }
}

inline void Engine_draw_tri_wireframe(struct Engine *e, const struct Tri t, int r, int g, int b) {
    Engine_bresenham(e, t.p1.x, t.p1.y, t.p2.x, t.p2.y, t.r, t.g, t.b);
    Engine_bresenham(e, t.p2.x, t.p2.y, t.p3.x, t.p3.y, t.r, t.g, t.b);
    Engine_bresenham(e, t.p3.x, t.p3.y, t.p1.x, t.p1.y, t.r, t.g, t.b);
}

void Engine_run(struct Engine *e) {
    printf("Engine_run: running engine.\n");

    // In general, we use left-handed coordinate systems.
    // That is, +x is leftwards.
    // Also,    +y is downwards.
    // Also,    +z is forwards

    // Models.
    struct Model *model = Model_unit_cube();
    Model_translate(model, 0, 0, 5);

    // Transformations.
    struct Matrix4 persp;
    Matrix4_perspective(90, e->aspect_ratio, 0.1, 10, &persp);
    //Matrix4_print(&persp);

    struct Matrix4 viewport;
    Matrix4_viewport(e->window_width, e->window_height, &viewport);
    //Matrix4_print(&raster);

    struct Matrix4 view;

    // Timing.
    Uint64 frame_start = 0;
    Uint64 frame_end   = SDL_GetPerformanceCounter();
    float dt = 0;
    //char fps_string[20];
    
    int running = 1;
    SDL_Event event;

    struct Vector3 camera_pos = Vector3_create(0, 0, 0);
    while (running) {
        frame_start = frame_end;
        frame_end   = SDL_GetPerformanceCounter();
        dt = (float)((frame_end - frame_start) * 1000 / (float)SDL_GetPerformanceFrequency());
        //sprintf(fps_string, "Impromptu | FPS: %d", (int)(1000.0 / dt));
        //SDL_SetWindowTitle(e->window, fps_string);

        // Handle user input events.
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_w) {
                    camera_pos.z += 0.1;
                } else if (event.key.keysym.sym == SDLK_s) {
                    camera_pos.z -= 0.1;
                } else if (event.key.keysym.sym == SDLK_d) {
                    camera_pos.x += 0.1;
                } else if (event.key.keysym.sym == SDLK_a) {
                    camera_pos.x -= 0.1;
                }
            }
        }

        // Model manipulation.
        Model_rotate(model, 0.09 * dt, 0.099 * dt, 0.0999 * dt);

        // Clear frame.
        //SDL_RenderClear(e->renderer);
        memset(e->color_buffer, 20, e->color_buffer_size);
        
        // Draw the object.
        for (int i = 0; i < model->mesh.num_tris; ++i) {
            struct Tri t = model->mesh.tris[i]; // t is a copy.
            
            // --- MODEL-LOCAL SPACE ----

            // Apply the model matrix to convert the local Tri vertices to world coordinates.
            t.p1 = Vector3_mul_Matrix4(t.p1, &model->model_to_world);
            t.p2 = Vector3_mul_Matrix4(t.p2, &model->model_to_world);
            t.p3 = Vector3_mul_Matrix4(t.p3, &model->model_to_world);

            // --- WORLD SPACE ----

            // Compute and apply view matrix.
            // TODO: recompute only when view has changed.
            Matrix4_look_at(
                camera_pos, 
                Vector3_create(0, 0, camera_pos.z + 1),
                Vector3_create(0, 1, 0), 
                &view
            );

            t.p1 = Vector3_mul_Matrix4(t.p1, &view);
            t.p2 = Vector3_mul_Matrix4(t.p2, &view);
            t.p3 = Vector3_mul_Matrix4(t.p3, &view);

            // --- CAMERA SPACE ----

            // Apply perspective projection matrix.
            t.p1 = Vector3_mul_Matrix4(t.p1, &persp);
            t.p2 = Vector3_mul_Matrix4(t.p2, &persp);
            t.p3 = Vector3_mul_Matrix4(t.p3, &persp);

            // --- HOMOGENEOUS CLIP SPACE ----            
            
            // The w coordinates have been changed by the perspective transformation.
            // The previous perspective transformation has transformed the view frustrum 
            // planes into a cube spanning (-w, w).

            // Perform clipping (i.e. the removal of triangles outside of
            // the clipping bounds and reconstruction of triangles partially
            // outside of clipping bounds).

            // Why do we perform clipping here and not after w division?
            // Division is expensive, so we want to do as little as possible (i.e. after clipping).

            if (t.p1.x < -t.p1.w) {
                printf("OUT LEFT\n");
            }
            if (t.p1.x > t.p1.w) {
                printf("OUT RIGHT\n");
            }
            if (t.p1.y < -t.p1.w) {
                printf("OUT TOP\n");
            }
            if (t.p1.y > t.p1.w) {
                printf("OUT BOT\n");
            }
            if (t.p1.z > t.p1.w) {
                printf("OUT FAR\n");
                printf("%f\n", t.p1.z);
            }
            if (t.p1.z < -t.p1.w) {
                printf("OUT NEAR\n");
                printf("%f\n", t.p1.z);
            }
            
            // Convert homogeneous coordinates to Cartesian for rasterization
            // by performing what is called w/homogeneous/perspective division.
            t.p1 = Vector3_smul(t.p1, 1 / t.p1.w);
            t.p2 = Vector3_smul(t.p2, 1 / t.p2.w);
            t.p3 = Vector3_smul(t.p3, 1 / t.p3.w);

            // --- NDC (Normalized Device Coordinate) SPACE ----

            // Apply viewport transform to obtain screen coordinates.
            t.p1 = Vector3_mul_Matrix4(t.p1, &viewport);
            t.p2 = Vector3_mul_Matrix4(t.p2, &viewport);
            t.p3 = Vector3_mul_Matrix4(t.p3, &viewport);

            // --- SCREEN SPACE ----

            // Perform rasterization of triangle.
            Engine_draw_tri_wireframe(e, t, 255, 255 ,255);
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

    // TEMPORARY:
    Model_destroy(model);
}
