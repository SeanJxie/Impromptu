#include "model.h"

struct Model *Model_create(struct Tri *mesh, int num_tris, float x, float y, float z, float rx, float ry, float rz, float sx, float sy, float sz) {
    struct Model *out = malloc(sizeof(struct Model));

    // Build the model matrix.
    struct Matrix4 translate;
    Matrix4_translate(x, y, z, &translate);
    struct Matrix4 rotate;
    Matrix4_rotate_xyz(rx, ry,rz, &rotate);
    struct Matrix4 scale;
    Matrix4_scale(sx, sy, sz, &scale);

    // Model matrix is, in order: scale, rotate, translate.
    struct Matrix4 scale_then_rotate;
    Matrix4_mul(&rotate, &scale, &scale_then_rotate);
    Matrix4_mul(&translate, &scale_then_rotate, &out->model_to_world);

    out->mesh = mesh;
    out->num_tris = num_tris;

    return out;
}

// TODO: Cleanup!
struct Model *Model_from_obj(const char *file_name, float x, float y, float z, float rx, float ry, float rz, float sx, float sy, float sz) {
    struct Model *out = Model_create(NULL, 0, x, y, z, rx, ry, rz, sx, sy, sz);

    int num_vertices = 0;
    int num_tris     = 0;

    char line[MAX_LEN_LINE];
    FILE *fp = fopen(file_name, "r");

    // Count vertices and faces.
    while (fgets(line, MAX_LEN_LINE, fp)) {
        if (line[0] == 'v') {
            ++num_vertices;
        } else if (line[0] == 'f') {
            ++num_tris;
        }
    }

    fseek(fp, 0, SEEK_SET);
    
    // Storage.
    struct Vector3 *vertices = malloc(sizeof(struct Vector3) * num_vertices);
    struct Tri     *tris     = malloc(sizeof(struct Tri)     * num_tris);

    int vertices_i = 0;
    int tris_i     = 0;
    
    char s[MAX_NUM_SUBSTR][MAX_LEN_SUBSTR];
    int  len_s;

    struct Obj_mtl curr_mtl;
    struct Obj_mtl mtl_lib[MAX_NUM_MTL];
    int            num_mtl;

    // Load vertices and faces to storage.
    while (fgets(line, MAX_LEN_LINE, fp)) {
        split(line, s, &len_s);

        if (strcmp(s[0], "v") == 0) {
            // struct Vector3 v;
            // parse_vertex(line, &v);
            vertices[vertices_i++] = Vector3_create_point(atof(s[1]), atof(s[2]), atof(s[3]));
        } else if (strcmp(s[0], "f") == 0) {
            // int i1, i2, i3;
            // parse_face(line, &i1, &i2, &i3);
            tris[tris_i++] = Tri_create(vertices[atoi(s[1]) - 1], vertices[atoi(s[2]) - 1], vertices[atoi(s[3]) - 1], 
                                        curr_mtl.kd.x * 255, curr_mtl.kd.y * 255, curr_mtl.kd.z * 255);
        } else if (strcmp(s[0], "mtllib") == 0) {
            // Load mtl library.
            parse_mtl(s[1], mtl_lib, &num_mtl);
        } else if (strcmp(s[0], "usemtl") == 0) {
            for (int i = 0; i < num_mtl; ++i) {
                if (strcmp(s[1], mtl_lib[i].id) == 0) {
                    curr_mtl = mtl_lib[i];
                }
            }
        }
    }

    fclose(fp);
    free(vertices);
    
    out->mesh = tris;
    out->num_tris = num_tris;

    return out;
}

void Model_destroy(struct Model *m) {
    free(m->mesh);
    free(m);
}

inline void Model_translate(struct Model *m, float delta_x, float delta_y, float delta_z) {
    struct Matrix4 translate;
    Matrix4_translate(delta_x, delta_y, delta_z, &translate);

    // We have to apply the model transform before translation since scaling and rotation
    // are relative to (0, 0, 0) in model coordinates.
    Matrix4_mul(&translate, &m->model_to_world, &m->model_to_world);
}

inline void Model_rotate(struct Model *m, float delta_rx, float delta_ry, float delta_rz) {
    struct Matrix4 rotate;
    Matrix4_rotate_xyz(delta_rx, delta_ry, delta_rz, &rotate);
    Matrix4_mul(&m->model_to_world, &rotate, &m->model_to_world);
}

inline void Model_scale(struct Model *m, float delta_sx, float delta_sy, float delta_sz) {
    struct Matrix4 scale;
    Matrix4_scale(delta_sx, delta_sy, delta_sz, &scale);
    Matrix4_mul(&m->model_to_world, &scale, &m->model_to_world);
}

struct Model *Model_unit_cube() {
    int num_tris = 12;
    struct Tri *mesh = malloc(sizeof(struct Tri) * num_tris);
    
    // These are in model-local coordinates.
    mesh[0] = Tri_create(
        Vector3_create_point(0, 0, 0), // Vertex 1.
        Vector3_create_point(0, 0, 1), // Vertex 2.
        Vector3_create_point(1, 0, 0), // Vertex 3.
        rand() % 256,                  // r
        rand() % 256,                  // g
        rand() % 256                   // b
    );
    mesh[1] = Tri_create(                            
        Vector3_create_point(1, 0, 1), 
        Vector3_create_point(1, 0, 0), 
        Vector3_create_point(0, 0, 1),
        rand() % 256,
        rand() % 256,
        rand() % 256  
    );
    mesh[2] = Tri_create(                            
        Vector3_create_point(0, 1, 0), 
        Vector3_create_point(0, 1, 1), 
        Vector3_create_point(1, 1, 0),
        rand() % 256,
        rand() % 256,
        rand() % 256  
    );
    mesh[3] = Tri_create(                            
        Vector3_create_point(1, 1, 1), 
        Vector3_create_point(1, 1, 0), 
        Vector3_create_point(0, 1, 1),
        rand() % 256,
        rand() % 256,
        rand() % 256  
    );
    mesh[4] = Tri_create(                           
        Vector3_create_point(0, 0, 1), 
        Vector3_create_point(0, 1, 1), 
        Vector3_create_point(1, 0, 1),
        rand() % 256,
        rand() % 256,
        rand() % 256
    );
    mesh[5] = Tri_create(                            
        Vector3_create_point(1, 1, 1), 
        Vector3_create_point(1, 0, 1), 
        Vector3_create_point(0, 1, 1),
        rand() % 256,
        rand() % 256,
        rand() % 256  
    );
    mesh[6] = Tri_create(                            
        Vector3_create_point(0, 0, 0), 
        Vector3_create_point(0, 1, 0), 
        Vector3_create_point(1, 0, 0),
        rand() % 256,
        rand() % 256,
        rand() % 256
    );
    mesh[7] = Tri_create(                            
        Vector3_create_point(1, 1, 0), 
        Vector3_create_point(1, 0, 0), 
        Vector3_create_point(0, 1, 0),
        rand() % 256,
        rand() % 256,
        rand() % 256  
    );
    mesh[8] = Tri_create(                            
        Vector3_create_point(0, 0, 0), 
        Vector3_create_point(0, 1, 0), 
        Vector3_create_point(0, 0, 1),
        rand() % 256,
        rand() % 256,
        rand() % 256  
    );
    mesh[9] = Tri_create(                            
        Vector3_create_point(0, 1, 1), 
        Vector3_create_point(0, 0, 1), 
        Vector3_create_point(0, 1, 0),
        rand() % 256,
        rand() % 256,
        rand() % 256  
    );
    mesh[10] = Tri_create(                            
        Vector3_create_point(1, 0, 0), 
        Vector3_create_point(1, 1, 0), 
        Vector3_create_point(1, 0, 1),
        rand() % 256,
        rand() % 256,
        rand() % 256  
    );
    mesh[11] = Tri_create(                            
        Vector3_create_point(1, 1, 1), 
        Vector3_create_point(1, 0, 1), 
        Vector3_create_point(1, 1, 0),
        rand() % 256,
        rand() % 256,
        rand() % 256  
    );

    // Make center of cube (0, 0, 0).
    struct Vector3 shift = Vector3_create_direction(-0.5, -0.5, -0.5);
    for (int i = 0; i < num_tris; ++i) {
        mesh[i].p1 = Vector3_add(mesh[i].p1, shift);
        mesh[i].p2 = Vector3_add(mesh[i].p2, shift);
        mesh[i].p3 = Vector3_add(mesh[i].p3, shift);
    }

    return Model_create(
        mesh,
        num_tris,
        0, 0, 0, 
        0, 0, 0, 
        1, 1, 1
    );
}

struct Model *Model_unit_triangle() {
    int num_tris = 1;

    struct Tri *mesh = malloc(sizeof(struct Tri) * num_tris);
    
    mesh[0] = Tri_create(
        Vector3_create_point(0, 0, 0), 
        Vector3_create_point(0, 1, 0), 
        Vector3_create_point(1, 0, 0), 
        rand() % 256,                  
        rand() % 256,                  
        rand() % 256                   
    );
    
    return Model_create(
        mesh,
        num_tris,
        0, 0, 0, 
        0, 0, 0, 
        1, 1, 1
    );
}

struct Model *Model_unit_square() {
    int num_tris = 2;

    struct Tri *mesh = malloc(sizeof(struct Tri) * num_tris);
    
    // CCW winding order
    mesh[0] = Tri_create(
        Vector3_create_point(0, 0, 0), 
        Vector3_create_point(1, 0, 0), 
        Vector3_create_point(0, 1, 0), 
        rand() % 256,                  
        rand() % 256,                  
        rand() % 256                   
    );
    mesh[1] = Tri_create(
        Vector3_create_point(1, 1, 0), 
        Vector3_create_point(0, 1, 0),
        Vector3_create_point(1, 0, 0), 
        rand() % 256,                  
        rand() % 256,                  
        rand() % 256                   
    );
    

    return Model_create(
        mesh,
        num_tris,
        0, 0, 0, 
        0, 0, 0, 
        1, 1, 1
    );
}