#include "engine.h"

struct Engine *Engine_create(int window_width, int window_height) {
    printf("Engine_create: Initializing engine.\n");

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_ShowCursor(0);

    struct Engine *e = malloc(sizeof(struct Engine));

    e->window_width       = window_width;
    e->window_height      = window_height;
    e->num_window_pixels  = window_width * window_height;
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
    for (int i = 0; i < e->num_window_pixels; ++i) {
        e->depth_buffer[i] = -1.0;
    }

    // Controls.
    e->move_speed = 0.005;
    e->look_speed = 0.0001;

    // Render options.
    e->wireframe           = 0;
    e->backface_culling    = 0;
    e->show_vertex_normals = 1;
    
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
        
        if (steep) {
            if (0 <= y && y < e->window_width && 0 <= x && x < e->window_height) Engine_set_pixel(e, y, x, r, g, b);
        } else {
            if (0 <= x && x < e->window_width && 0 <= y && y < e->window_height) Engine_set_pixel(e, x, y, r, g, b);
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

inline void Engine_raster_tri_wireframe(struct Engine *e, struct Vector3 v1, struct Vector3 v2, struct Vector3 v3, int r, int g, int b) {
    Engine_bresenham(e, v1.x, v1.y, v2.x, v2.y, r, g, b);
    Engine_bresenham(e, v2.x, v2.y, v3.x, v3.y, r, g, b);
    Engine_bresenham(e, v3.x, v3.y, v1.x, v1.y, r, g, b);
}

inline float _edge(float x1, float y1, float x2, float y2, float x3, float y3) {
    return (x3 - x1) * (y2 - y1) - (y3 - y1) * (x2 - x1);
}

void Engine_run(struct Engine *e) {
    printf("Engine_run: running engine.\n");

    // Models.
    struct Model *model = Model_from_obj(
        "models/sphere.obj", 
        0, 0, 1, 
        0, 0, 0, 
        1, 1, 1
    );
    //struct Model *model = Model_unit_cube();
    printf("Triangle count = %d\n", model->num_tris);

    // Lights.
    // struct LightSource point_light;
    // point_light.type  = LIGHT_TYPE_POINT;
    // point_light.power = 100;
    // float tmp_light_rotation_angle = 0;

    // Transformations.
    struct Matrix4 projection;
    Matrix4_perspective(90, e->aspect_ratio, 0.1, 10, &projection);

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
            } else if (event.type == SDL_KEYDOWN) {
                if      (event.key.keysym.sym == SDLK_w)      w_pressed      = 1;
                else if (event.key.keysym.sym == SDLK_s)      s_pressed      = 1;
                else if (event.key.keysym.sym == SDLK_a)      a_pressed      = 1;
                else if (event.key.keysym.sym == SDLK_d)      d_pressed      = 1;
                else if (event.key.keysym.sym == SDLK_LSHIFT) lshift_pressed = 1;
                else if (event.key.keysym.sym == SDLK_SPACE)  space_pressed  = 1;

                else if (event.key.keysym.sym == SDLK_r)      e->wireframe   = e->wireframe ? 0 : 1;
            } else if (event.type == SDL_KEYUP) {
                if      (event.key.keysym.sym == SDLK_w)      w_pressed      = 0;
                else if (event.key.keysym.sym == SDLK_s)      s_pressed      = 0;
                else if (event.key.keysym.sym == SDLK_a)      a_pressed      = 0;
                else if (event.key.keysym.sym == SDLK_d)      d_pressed      = 0;
                else if (event.key.keysym.sym == SDLK_LSHIFT) lshift_pressed = 0;
                else if (event.key.keysym.sym == SDLK_SPACE)  space_pressed  = 0;
            }
        }

        // --- CAMERA CONTROLS --- 
        
        // Mouse.
        SDL_GetMouseState(&mouse_x, &mouse_y);
        SDL_WarpMouseInWindow(e->window, e->half_window_width, e->half_window_height);

        look_angle_horizontal -= e->look_speed * dt * (e->half_window_width  - mouse_x);
        look_angle_vertical   -= e->look_speed * dt * (e->half_window_height - mouse_y);  

        // Clamp vertical to [-pi / 2, pi / 2].
        look_angle_vertical = MAX(-M_PI * 0.5, MIN(M_PI * 0.5, look_angle_vertical));

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

        //Model_rotate(model, 0, dt * 0.01, 0);

        // Recompute view matrix.
        Matrix4_look_at(
            camera_pos, 
            Vector3_add(camera_pos, look_forward),
            look_up, 
            &view
        );

        struct Matrix4 view_inv;
        struct Matrix4 view_inv_transpose;
        Matrix4_inverse(&view, &view_inv);
        Matrix4_transpose(&view_inv, &view_inv_transpose);


        struct Matrix4 model_inv;
        struct Matrix4 model_inv_transpose;
        Matrix4_inverse(&model->model_to_world, &model_inv);
        Matrix4_transpose(&model_inv, &model_inv_transpose);

        // Clear frame buffers.
        memset(e->color_buffer, 0, e->color_buffer_size);
        for (int i = 0; i < e->num_window_pixels; ++i) {
            e->depth_buffer[i] = -1.0;
        }

        // The following is something like a rendering pipeline. Specifically, the one specified in OpenGL.
        for (int i = 0; i < model->num_tris; ++i) {
            struct Tri t = model->mesh[i]; // Copy.

            struct Vector3 v0 = t.v0.pos;
            struct Vector3 v1 = t.v1.pos;
            struct Vector3 v2 = t.v2.pos;
            
            struct Vector3 c0 = t.v0.col;
            struct Vector3 c1 = t.v1.col;
            struct Vector3 c2 = t.v2.col;

            struct Vector3 n0 = t.v0.norm;
            struct Vector3 n1 = t.v1.norm;
            struct Vector3 n2 = t.v2.norm;
            
            // Apply Model-View-Projection (MVP) to the three vertices and normals.     

            // Model.       
            v0 = Matrix4_vmul(&model->model_to_world, v0);
            v1 = Matrix4_vmul(&model->model_to_world, v1);
            v2 = Matrix4_vmul(&model->model_to_world, v2);

            // Normals to world space via inverse transpose of model.
            n0 = Matrix4_vmul(&model->model_to_world, n0);
            n1 = Matrix4_vmul(&model->model_to_world, n1);
            n2 = Matrix4_vmul(&model->model_to_world, n2);

            if (e->backface_culling) {
                struct Vector3 plane_normal = Vector3_cross(Vector3_sub(v1, v0), Vector3_sub(v2, v0));
                struct Vector3 cam_ray = Vector3_sub(v0, camera_pos);

                // If face isn't facing camera, don't proceed (back-face culling).
                if (Vector3_dot(plane_normal, cam_ray) >= 0) {
                    continue;
                }
            }

            // For rendering normals. These are locations of vertices plus their normals.
            // They are treated as any other vertex from this point on and processed in the pipeline.
            struct Vector3 vn0 = Vector3_add(v0, n0);
            struct Vector3 vn1 = Vector3_add(v1, n1);
            struct Vector3 vn2 = Vector3_add(v2, n2);
            
            // View.
            v0 = Matrix4_vmul(&view, v0);
            v1 = Matrix4_vmul(&view, v1);
            v2 = Matrix4_vmul(&view, v2);

            vn0 = Matrix4_vmul(&view, vn0);
            vn1 = Matrix4_vmul(&view, vn1);
            vn2 = Matrix4_vmul(&view, vn2);

            // Projection.
            v0 = Matrix4_vmul(&projection, v0);
            v1 = Matrix4_vmul(&projection, v1);
            v2 = Matrix4_vmul(&projection, v2);

            vn0 = Matrix4_vmul(&projection, vn0);
            vn1 = Matrix4_vmul(&projection, vn1);
            vn2 = Matrix4_vmul(&projection, vn2);

            // -- CULL IN CLIP SPACE --
            // 
            // If the entire triangle is outside the view frustrum, don't even bother with
            // perspective divide and rasterization.

            // Entire triangle is out left.
            if (v0.x < -v0.w && 
                v1.x < -v1.w && 
                v2.x < -v2.w) {
                continue;
            }

            // Entire triangle is out right.
            if (v0.x > v0.w && 
                v1.x > v1.w && 
                v2.x > v2.w) {
                continue;
            }

            // Entire triangle is out top.
            if (v0.y < -v0.w && 
                v1.y < -v1.w && 
                v2.y < -v2.w) {
                continue;
            }

            // Entire triangle is out bottom.
            if (v0.y > v0.w && 
                v1.y > v1.w && 
                v2.y > v2.w) {
                continue;
            }

            // Triangle is out near.
            // Cull the triangle even if only one vertex is out.
            if (v0.z < 0 || 
                v1.z < 0 || 
                v2.z < 0) {
                continue;
            }

            // Entire triangle is out far.
            if (v0.z > v0.w && 
                v1.z > v1.w && 
                v2.z > v2.w) {
                continue;
            }

            // --- PERSPECTIVE DIVIDE ---

            v0 = Vector3_smul(v0, 1 / v0.w);
            v1 = Vector3_smul(v1, 1 / v1.w);
            v2 = Vector3_smul(v2, 1 / v2.w);

            vn0 = Vector3_smul(vn0, 1 / vn0.w);
            vn1 = Vector3_smul(vn1, 1 / vn1.w);
            vn2 = Vector3_smul(vn2, 1 / vn2.w);
            

            // --- NORMALIZED DEVICE COORDINATE SPACE ----

            // Apply viewport transform to obtain screen coordinates.
            v0 = Matrix4_vmul(&viewport, v0);
            v1 = Matrix4_vmul(&viewport, v1);
            v2 = Matrix4_vmul(&viewport, v2);

            vn0 = Matrix4_vmul(&viewport, vn0);
            vn1 = Matrix4_vmul(&viewport, vn1);
            vn2 = Matrix4_vmul(&viewport, vn2);

            // --- SCREEN SPACE ----

            // Perform rasterization of triangle.
            if (e->wireframe) {
                Engine_raster_tri_wireframe(e, v0, v1, v2, 255, 255, 255);
                continue;
            }

            // --- RASTERIZE TRIANGLE ---

            // Draw vertex normals.
            if (e->show_vertex_normals) {
                Engine_bresenham(e, v0.x, v0.y, vn0.x, vn0.y, 255, 255, 255);
                Engine_bresenham(e, v1.x, v1.y, vn1.x, vn1.y, 255, 255, 255);
                Engine_bresenham(e, v2.x, v2.y, vn2.x, vn2.y, 255, 255, 255);
            }
            
            float x1 = v0.x;
            float y1 = v0.y;
            float z1 = v0.z;

            float x2 = v1.x;
            float y2 = v1.y;
            float z2 = v1.z;

            float x3 = v2.x;
            float y3 = v2.y;
            float z3 = v2.z;

            // Transform unit normals (components in range [-1, 1]) to be suitable for colouring (components in range [0, 1]).
            n0 = Vector3_add(Vector3_smul(Vector3_normalize(n0), 0.5), (struct Vector3){0.5, 0.5, 0.5});
            n1 = Vector3_add(Vector3_smul(Vector3_normalize(n1), 0.5), (struct Vector3){0.5, 0.5, 0.5});
            n2 = Vector3_add(Vector3_smul(Vector3_normalize(n2), 0.5), (struct Vector3){0.5, 0.5, 0.5});

            // Finding bounding box of triangle (also considering the bounds of the screen).
            float bb_min_x = MAX(MIN(MIN(x1, x2), x3), 0);
            float bb_max_x = MIN(MAX(MAX(x1, x2), x3), e->window_width);
            float bb_min_y = MAX(MIN(MIN(y1, y2), y3), 0);
            float bb_max_y = MIN(MAX(MAX(y1, y2), y3), e->window_height);
            
            float px, py;
            float w0, w1, w2;
            float buffer_depth;

            float area_inv = 1 / _edge(x1, y1, x2, y2, x3, y3);

            // Interpolated values.
            float z;
            int r, g, b;
            float nx, ny, nz;

            for (int y = bb_min_y; y < bb_max_y; ++y) {
                for (int x = bb_min_x; x < bb_max_x; ++x) {
                    px = x + 0.5;
                    py = y + 0.5;
                    w0 = _edge(x2, y2, x3, y3, px, py);
                    w1 = _edge(x3, y3, x1, y1, px, py);
                    w2 = _edge(x1, y1, x2, y2, px, py);
                    if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                        // Barycentric coordinates.
                        w0 *= area_inv;
                        w1 *= area_inv;
                        w2 *= area_inv;

                        // Interpolate depth.
                        z = z1 * w0 + z2 * w1 + z3 * w2;

                        // Interpolate colour.
                        r = c0.x * w0 + c1.x * w1 + c2.x * w2;
                        g = c0.y * w0 + c1.y * w1 + c2.y * w2;
                        b = c0.z * w0 + c1.z * w1 + c2.z * w2;

                        // Interpolate normal.
                        nx = n0.x * w0 + n1.x * w1 + n2.x * w2;
                        ny = n0.y * w0 + n1.y * w1 + n2.y * w2;
                        nz = n0.z * w0 + n1.z * w1 + n2.z * w2;

                        buffer_depth = e->depth_buffer[(e->window_width * y) + x];

                        // Depth test.
                        if (z < buffer_depth || buffer_depth == -1) {
                            // The following is something like a fragment shader.
                            Engine_set_pixel(e, x, y, nx * 255, ny * 255, nz * 255);
                            Engine_set_depth(e, x, y, z);
                        }
                    }
                }
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
