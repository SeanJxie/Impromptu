#include "engine.h"

struct Engine *Engine_create(int window_width, int window_height) {
    printf("Engine_create: Initializing engine.\n");

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_ShowCursor(0);

    struct Engine *e = malloc(sizeof(struct Engine));

    e->window_width       = window_width;
    e->window_height      = window_height;
    e->half_window_width  = window_width * 0.5;
    e->half_window_height = window_height * 0.5;
    e->aspect_ratio       = (float)window_width / window_height;

    // Window and renderer.
    e->window = SDL_CreateWindow(
        "Impromptu", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        e->window_width, e->window_height, 
        SDL_WINDOW_SHOWN
    );
    e->renderer = SDL_CreateRenderer(e->window, -1, SDL_RENDERER_ACCELERATED);
    
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
    // memset float arary.
    for (int i = 0; i < window_width * window_height; ++i) {
        e->depth_buffer[i] = -1.0;
    }

    // Controls.
    e->move_speed = 0.01;
    e->look_speed = 0.0001;

    // Render options.
    e->raster_wireframe = 0;
    e->backface_culling = 1;
    e->show_normals     = 1;
    
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
    // TODO: Not safe. Ensure a valid buffer index.
    int offset = (e->window_width * y * 4) + x * 4;
    e->color_buffer[offset + 0] = r;
    e->color_buffer[offset + 1] = g;
    e->color_buffer[offset + 2] = b;
    e->color_buffer[offset + 3] = 255;
}

inline void Engine_set_depth(struct Engine *e, int x, int y, float depth) {
    e->depth_buffer[(e->window_width * y) + x] = depth;
}

inline float Engine_get_depth(struct Engine *e, int x, int y) {
    return e->depth_buffer[(e->window_width * y) + x];
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
        if (0 <= x && x < e->window_width && 0 <= y && y < e->window_height) {
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
}

inline void Engine_raster_tri_wireframe(struct Engine *e, struct Vector3 v1, struct Vector3 v2, struct Vector3 v3, int r, int g, int b) {
    Engine_bresenham(e, v1.x, v1.y, v2.x, v2.y, r, g, b);
    Engine_bresenham(e, v2.x, v2.y, v3.x, v3.y, r, g, b);
    Engine_bresenham(e, v3.x, v3.y, v1.x, v1.y, r, g, b);
}

inline float _edge(float x1, float y1, float x2, float y2, float x3, float y3) {
    return (x3 - x1) * (y2 - y1) - (y3 - y1) * (x2 - x1);
}

inline void Engine_raster_tri(struct Engine *e, struct Vector3 v1, struct Vector3 v2, struct Vector3 v3, int r, int g, int b) {
    float x1 = v1.x;
    float y1 = v1.y;

    float x2 = v2.x;
    float y2 = v2.y;

    float x3 = v3.x;
    float y3 = v3.y;
    
    // Calculate equation of plane in which the triangle lies for depth comparison.
    struct Vector3 plane_normal = Vector3_cross(Vector3_sub(v2, v1), Vector3_sub(v3, v1));

    float a  = plane_normal.x;
    float b_ = plane_normal.y;
    float c  = plane_normal.z;
    float d  = -(a * x1 + b_ * y1 + c * v1.z);

    // Evaluate for points within bounding box of triangle.
    float bb_min_x = MAX(MIN(MIN(x1, x2), x3), 0);
    float bb_max_x = MIN(MAX(MAX(x1, x2), x3), e->window_width);
    float bb_min_y = MAX(MIN(MIN(y1, y2), y3), 0);
    float bb_max_y = MIN(MAX(MAX(y1, y2), y3), e->window_height);
    
    float px, py;
    float z;
    for (int y = bb_min_y; y < bb_max_y; ++y) {
        for (int x = bb_min_x; x < bb_max_x; ++x) {
            px = x + 0.5;
            py = y + 0.5;
            if (_edge(x2, y2, x3, y3, px, py) >= 0 && 
                _edge(x3, y3, x1, y1, px, py) >= 0 && 
                _edge(x1, y1, x2, y2, px, py) >= 0) 
            {
                z = (-d - a * x - b_ * y) / c;
                float depth = e->depth_buffer[(e->window_width * y) + x];
                if (z < depth || depth == -1) {
                    //Engine_set_pixel(e, x, y, z * 255, z * 255, z * 255);
                    Engine_set_pixel(e, x, y, r, g, b);
                    Engine_set_depth(e, x, y, z);
                }
            }
        }
    }
}

void Engine_run(struct Engine *e) {
    printf("Engine_run: running engine.\n");

    // Models.
    struct Model *model = Model_from_obj(
        "models/suzanne.obj", 
        0, 0, 5, 
        0, 0, 0, 
        1, 1, 1
    );
    //struct Model *model = Model_unit_cube();
    printf("Triangle count = %d\n", model->num_tris);

    // Transformations.
    struct Matrix4 projection;
    Matrix4_perspective(90, e->aspect_ratio, 1, 100, &projection);

    struct Matrix4 view;
    struct Vector3 camera_pos = Vector3_create_point(0, 0, 0);
    Matrix4_look_at(
        camera_pos, 
        Vector3_create_point(0, 0, camera_pos.z + 1),
        Vector3_create_direction(0, 1, 0), 
        &view
    );

    struct Matrix4 viewport;
    Matrix4_viewport(e->window_width, e->window_height, &viewport);

    // Timing.
    Uint64 frame_start = 0;
    Uint64 frame_end   = SDL_GetPerformanceCounter();
    float dt = 0;
    char fps_string[20];
    
    // Control.
    int w_pressed      = 0;
    int a_pressed      = 0;
    int s_pressed      = 0;
    int d_pressed      = 0;
    int lshift_pressed = 0;
    int space_pressed  = 0;
    struct Vector3 move_direction = Vector3_create_direction(0, 0, 0);

    int mouse_x, mouse_y;
    // Looking down positive z by default.
    float look_angle_horizontal = 0;
    float look_angle_vertical   = 0;
    struct Vector3 look_forward = Vector3_create_direction(0, 0, 1);
    struct Vector3 look_right   = Vector3_create_direction(1, 0, 0);
    struct Vector3 look_up      = Vector3_create_direction(0, 1, 0);

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
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                running = 0;
                break;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_w) {
                    w_pressed = 1;
                } 
                if (event.key.keysym.sym == SDLK_s) {
                    s_pressed = 1;
                } 
                if (event.key.keysym.sym == SDLK_a) {
                    a_pressed = 1;
                } 
                if (event.key.keysym.sym == SDLK_d) {
                    d_pressed = 1;
                } 
                if (event.key.keysym.sym == SDLK_LSHIFT) {
                    lshift_pressed = 1;
                } 
                if (event.key.keysym.sym == SDLK_SPACE) {
                    space_pressed = 1;
                }
            } else if (event.type == SDL_KEYUP) {
                if (event.key.keysym.sym == SDLK_w) {
                    w_pressed = 0;
                } 
                if (event.key.keysym.sym == SDLK_s) {
                    s_pressed = 0;
                } 
                if (event.key.keysym.sym == SDLK_a) {
                    a_pressed = 0;
                } 
                if (event.key.keysym.sym == SDLK_d) {
                    d_pressed = 0;
                } 
                if (event.key.keysym.sym == SDLK_LSHIFT) {
                    lshift_pressed = 0;
                } 
                if (event.key.keysym.sym == SDLK_SPACE) {
                    space_pressed = 0;
                }
            }
        }

        // --- CAMERA CONTROLS --- 
        
        // Mouse.
        SDL_GetMouseState(&mouse_x, &mouse_y);
        SDL_WarpMouseInWindow(e->window, e->half_window_width, e->half_window_height);

        look_angle_horizontal -= e->look_speed * dt * (e->half_window_width  - mouse_x);
        look_angle_vertical   -= e->look_speed * dt * (e->half_window_height - mouse_y);  

        look_forward.x = cosf(look_angle_vertical) * sinf(look_angle_horizontal);
        look_forward.y = sinf(look_angle_vertical);
        look_forward.z = cosf(look_angle_vertical) * cosf(look_angle_horizontal);
        look_forward   = Vector3_normalize(look_forward);

        look_right.x = sinf(look_angle_horizontal - M_PI * 0.5);
        look_right.y = 0;
        look_right.z = cosf(look_angle_horizontal - M_PI * 0.5);
        look_right   = Vector3_normalize(look_right);

        look_up = Vector3_cross(look_right, look_forward);

        // Keyboard.
        if (w_pressed) {
            move_direction = Vector3_smul(look_forward, dt * e->move_speed);
            camera_pos     = Vector3_add(camera_pos, move_direction);
        } 
        if (s_pressed) {
            move_direction = Vector3_smul(look_forward, dt * -e->move_speed);
            camera_pos     = Vector3_add(camera_pos, move_direction);
        } 
        if (a_pressed) {
            move_direction = Vector3_smul(look_right, dt * e->move_speed);
            camera_pos     = Vector3_add(camera_pos, move_direction);
        } 
        if (d_pressed) {
            move_direction = Vector3_smul(look_right, dt * -e->move_speed);
            camera_pos     = Vector3_add(camera_pos, move_direction);
        } 
        if (lshift_pressed) {
            move_direction = Vector3_smul(Vector3_create_direction(0, 1, 0), dt * e->move_speed);
            camera_pos     = Vector3_add(camera_pos, move_direction);
        }
        if (space_pressed) {
            move_direction = Vector3_smul(Vector3_create_direction(0, 1, 0), dt * -e->move_speed);
            camera_pos     = Vector3_add(camera_pos, move_direction);
        } 

        Model_rotate(model, 0, dt * 0.01, 0);

        // Recompute view matrix.
        Matrix4_look_at(
            camera_pos, 
            Vector3_add(camera_pos, look_forward),
            look_up, 
            &view
        );

        // Clear frame.
        memset(e->color_buffer, 20, e->color_buffer_size);
        for (int i = 0; i < e->window_width * e->window_height; ++i) {
            e->depth_buffer[i] = -1.0;
        }

        // Draw the object.
        for (int i = 0; i < model->num_tris; ++i) {
            struct Tri t = model->mesh[i]; // t is a copy.
            struct Vector3 vert1 = t.p1;
            struct Vector3 vert2 = t.p2;
            struct Vector3 vert3 = t.p3;

            // Apply Model-View-Projection (MVP) to the three vertices and normal.            
            vert1 = Matrix4_vmul(&model->model_to_world, vert1);
            vert2 = Matrix4_vmul(&model->model_to_world, vert2);
            vert3 = Matrix4_vmul(&model->model_to_world, vert3);

            struct Vector3 plane_normal = Vector3_cross(Vector3_sub(vert2, vert1), Vector3_sub(vert3, vert1));
            struct Vector3 cam_ray = Vector3_sub(vert1, camera_pos);

            // If face isn't facing camera, don't proceed (back-face culling).
            if (Vector3_dot(plane_normal, cam_ray) >= 0) {
                continue;
            }

            vert1 = Matrix4_vmul(&view, vert1);
            vert2 = Matrix4_vmul(&view, vert2);
            vert3 = Matrix4_vmul(&view, vert3);

            vert1 = Matrix4_vmul(&projection, vert1);
            vert2 = Matrix4_vmul(&projection, vert2);
            vert3 = Matrix4_vmul(&projection, vert3);

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
            // Temporary. Cull the triangle even
            // if only one vertex is out near.
            if (vert1.z < 0 || 
                vert2.z < 0 || 
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
            // TODO: Important for models with large polygons.
            // if ((vert1.z < 0 && vert2.z >= 0 && vert3.z >= 0) || 
            //     (vert2.z < 0 && vert1.z >= 0 && vert3.z >= 0) ||
            //     (vert3.z < 0 && vert1.z >= 0 && vert2.z >= 0)) {

                
            // }
            
            vert1 = Vector3_smul(vert1, 1 / vert1.w);
            vert2 = Vector3_smul(vert2, 1 / vert2.w);
            vert3 = Vector3_smul(vert3, 1 / vert3.w);

            // --- NDC (Normalized Device Coordinate) SPACE ----

            // Apply viewport transform to obtain screen coordinates.
            vert1 = Matrix4_vmul(&viewport, vert1);
            vert2 = Matrix4_vmul(&viewport, vert2);
            vert3 = Matrix4_vmul(&viewport, vert3);

            // --- SCREEN SPACE ----
            // Perform rasterization of triangle.
            if (e->raster_wireframe) {
                Engine_raster_tri_wireframe(e, vert1, vert2, vert3, 255, 255, 255);
            } else {
                Engine_raster_tri(e, vert1, vert2, vert3, t.r, t.g, t.b);
            }            
        }

        // Copy pixels to texture.
        //SDL_UpdateTexture(e->frame_texture, NULL, e->color_buffer, e->window_width * 4);
        unsigned char *locked_pixels;
        int pitch; // Dummy.
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
}
