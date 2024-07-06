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

struct Model *Model_from_obj(const char *file_name, float x, float y, float z, float rx, float ry, float rz, float sx, float sy, float sz) {
    int num_tris;
    struct Tri *mesh = parse_obj(file_name, &num_tris);
    struct Model *out = Model_create(mesh, num_tris, x, y, z, rx, ry, rz, sx, sy, sz);

    return out;
}

void Model_destroy(struct Model *m) {
    free(m->mesh);
    free(m->map_kd);
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

// struct Model *Model_unit_cube() {
//     int num_tris = 12;
//     struct Tri *mesh = malloc(sizeof(struct Tri) * num_tris);
    
//     // These are in model-local coordinates.
//     mesh[0] = Tri_create(
//         Vector3_create_point(0, 0, 0), // Vertex 1.
//         Vector3_create_point(0, 0, 1), // Vertex 2.
//         Vector3_create_point(1, 0, 0), // Vertex 3.
//         rand() % 256,                  // r
//         rand() % 256,                  // g
//         rand() % 256                   // b
//     );
//     mesh[1] = Tri_create(                            
//         Vector3_create_point(1, 0, 1), 
//         Vector3_create_point(1, 0, 0), 
//         Vector3_create_point(0, 0, 1),
//         rand() % 256,
//         rand() % 256,
//         rand() % 256  
//     );
//     mesh[2] = Tri_create(                            
//         Vector3_create_point(0, 1, 0), 
//         Vector3_create_point(0, 1, 1), 
//         Vector3_create_point(1, 1, 0),
//         rand() % 256,
//         rand() % 256,
//         rand() % 256  
//     );
//     mesh[3] = Tri_create(                            
//         Vector3_create_point(1, 1, 1), 
//         Vector3_create_point(1, 1, 0), 
//         Vector3_create_point(0, 1, 1),
//         rand() % 256,
//         rand() % 256,
//         rand() % 256  
//     );
//     mesh[4] = Tri_create(                           
//         Vector3_create_point(0, 0, 1), 
//         Vector3_create_point(0, 1, 1), 
//         Vector3_create_point(1, 0, 1),
//         rand() % 256,
//         rand() % 256,
//         rand() % 256
//     );
//     mesh[5] = Tri_create(                            
//         Vector3_create_point(1, 1, 1), 
//         Vector3_create_point(1, 0, 1), 
//         Vector3_create_point(0, 1, 1),
//         rand() % 256,
//         rand() % 256,
//         rand() % 256  
//     );
//     mesh[6] = Tri_create(                            
//         Vector3_create_point(0, 0, 0), 
//         Vector3_create_point(0, 1, 0), 
//         Vector3_create_point(1, 0, 0),
//         rand() % 256,
//         rand() % 256,
//         rand() % 256
//     );
//     mesh[7] = Tri_create(                            
//         Vector3_create_point(1, 1, 0), 
//         Vector3_create_point(1, 0, 0), 
//         Vector3_create_point(0, 1, 0),
//         rand() % 256,
//         rand() % 256,
//         rand() % 256  
//     );
//     mesh[8] = Tri_create(                            
//         Vector3_create_point(0, 0, 0), 
//         Vector3_create_point(0, 1, 0), 
//         Vector3_create_point(0, 0, 1),
//         rand() % 256,
//         rand() % 256,
//         rand() % 256  
//     );
//     mesh[9] = Tri_create(                            
//         Vector3_create_point(0, 1, 1), 
//         Vector3_create_point(0, 0, 1), 
//         Vector3_create_point(0, 1, 0),
//         rand() % 256,
//         rand() % 256,
//         rand() % 256  
//     );
//     mesh[10] = Tri_create(                            
//         Vector3_create_point(1, 0, 0), 
//         Vector3_create_point(1, 1, 0), 
//         Vector3_create_point(1, 0, 1),
//         rand() % 256,
//         rand() % 256,
//         rand() % 256  
//     );
//     mesh[11] = Tri_create(                            
//         Vector3_create_point(1, 1, 1), 
//         Vector3_create_point(1, 0, 1), 
//         Vector3_create_point(1, 1, 0),
//         rand() % 256,
//         rand() % 256,
//         rand() % 256  
//     );

//     // Make center of cube (0, 0, 0).
//     struct Vector3 shift = Vector3_create_direction(-0.5, -0.5, -0.5);
//     for (int i = 0; i < num_tris; ++i) {
//         mesh[i].p1 = Vector3_add(mesh[i].p1, shift);
//         mesh[i].p2 = Vector3_add(mesh[i].p2, shift);
//         mesh[i].p3 = Vector3_add(mesh[i].p3, shift);
//     }

//     return Model_create(
//         mesh,
//         num_tris,
//         0, 0, 0, 
//         0, 0, 0, 
//         1, 1, 1
//     );
// }

// struct Model *Model_unit_triangle() {
//     int num_tris = 1;

//     struct Tri *mesh = malloc(sizeof(struct Tri) * num_tris);
    
//     mesh[0] = Tri_create(
//         Vector3_create_point(0, 0, 0), 
//         Vector3_create_point(0, 1, 0), 
//         Vector3_create_point(1, 0, 0), 
//         rand() % 256,                  
//         rand() % 256,                  
//         rand() % 256                   
//     );
    
//     return Model_create(
//         mesh,
//         num_tris,
//         0, 0, 0, 
//         0, 0, 0, 
//         1, 1, 1
//     );
// }

// struct Model *Model_unit_square() {
//     int num_tris = 2;

//     struct Tri *mesh = malloc(sizeof(struct Tri) * num_tris);
    
//     // CCW winding order
//     mesh[0] = Tri_create(
//         Vector3_create_point(0, 0, 0), 
//         Vector3_create_point(1, 0, 0), 
//         Vector3_create_point(0, 1, 0), 
//         rand() % 256,                  
//         rand() % 256,                  
//         rand() % 256                   
//     );
//     mesh[1] = Tri_create(
//         Vector3_create_point(1, 1, 0), 
//         Vector3_create_point(0, 1, 0),
//         Vector3_create_point(1, 0, 0), 
//         rand() % 256,                  
//         rand() % 256,                  
//         rand() % 256                   
//     );
    

//     return Model_create(
//         mesh,
//         num_tris,
//         0, 0, 0, 
//         0, 0, 0, 
//         1, 1, 1
//     );
// }