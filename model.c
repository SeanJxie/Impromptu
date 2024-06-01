#include "model.h"

struct Model *Model_create(float x, float y, float z, float rx, float ry, float rz, float sx, float sy, float sz, struct Mesh mesh) {
    struct Model *out = malloc(sizeof(struct Model));

    // Build the model matrix.
    struct Matrix4 translate;
    Matrix4_translate(x, y, z, &translate);
    struct Matrix4 rotate;
    Matrix4_rotate_xyz(rx, ry,rz, &rotate);
    struct Matrix4 scale;
    Matrix4_scale(sx, sy, sz, &scale);

    // Model matrix is, in order, scale, rotate, translate.
    struct Matrix4 scale_then_rotate;
    Matrix4_mul(&rotate, &scale, &scale_then_rotate);
    Matrix4_mul(&translate, &scale_then_rotate, &out->model_to_world);

    out->mesh = mesh;

    return out;
}

void Model_destroy(struct Model *m) {
    free(m->mesh.tris);
    free(m);
}

inline void Model_translate(struct Model *m, float delta_x, float delta_y, float delta_z) {
    struct Matrix4 translate;
    Matrix4_translate(delta_x, delta_y, delta_z, &translate);
    Matrix4_mul(&m->model_to_world, &translate, &m->model_to_world);
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
    struct Mesh model_mesh;
    model_mesh.num_tris = 12;

    struct Tri *mesh_tris = malloc(sizeof(struct Tri) * model_mesh.num_tris);
    
    // These are in model-local coordinates.
    mesh_tris[0] = (struct Tri){
        Vector3_create(0, 0, 0), // Vertex 1.
        Vector3_create(0, 0, 1), // Vertex 2.
        Vector3_create(1, 0, 0), // Vertex 3.
        rand() % 256,            // r
        rand() % 256,            // g
        rand() % 256             // b
    };
    mesh_tris[1] = (struct Tri){                            
        Vector3_create(1, 0, 1), 
        Vector3_create(1, 0, 0), 
        Vector3_create(0, 0, 1),
        rand() % 256,
        rand() % 256,
        rand() % 256  
    };
    mesh_tris[2] = (struct Tri){                            
        Vector3_create(0, 1, 0), 
        Vector3_create(0, 1, 1), 
        Vector3_create(1, 1, 0),
        rand() % 256,
        rand() % 256,
        rand() % 256  
    };
    mesh_tris[3] = (struct Tri){                            
        Vector3_create(1, 1, 1), 
        Vector3_create(1, 1, 0), 
        Vector3_create(0, 1, 1),
        rand() % 256,
        rand() % 256,
        rand() % 256  
    };
    mesh_tris[4] = (struct Tri){                           
        Vector3_create(0, 0, 1), 
        Vector3_create(0, 1, 1), 
        Vector3_create(1, 0, 1),
        rand() % 256,
        rand() % 256,
        rand() % 256
    };
    mesh_tris[5] = (struct Tri){                            
        Vector3_create(1, 1, 1), 
        Vector3_create(1, 0, 1), 
        Vector3_create(0, 1, 1),
        rand() % 256,
        rand() % 256,
        rand() % 256  
    };
    mesh_tris[6] = (struct Tri){                            
        Vector3_create(0, 0, 0), 
        Vector3_create(0, 1, 0), 
        Vector3_create(1, 0, 0),
        rand() % 256,
        rand() % 256,
        rand() % 256
    };
    mesh_tris[7] = (struct Tri){                            
        Vector3_create(1, 1, 0), 
        Vector3_create(1, 0, 0), 
        Vector3_create(0, 1, 0),
        rand() % 256,
        rand() % 256,
        rand() % 256  
    };
    mesh_tris[8] = (struct Tri){                            
        Vector3_create(0, 0, 0), 
        Vector3_create(0, 1, 0), 
        Vector3_create(0, 0, 1),
        rand() % 256,
        rand() % 256,
        rand() % 256  
    };
    mesh_tris[9] = (struct Tri){                            
        Vector3_create(0, 1, 1), 
        Vector3_create(0, 0, 1), 
        Vector3_create(0, 1, 0),
        rand() % 256,
        rand() % 256,
        rand() % 256  
    };
    mesh_tris[10] = (struct Tri){                            
        Vector3_create(1, 0, 0), 
        Vector3_create(1, 1, 0), 
        Vector3_create(1, 0, 1),
        rand() % 256,
        rand() % 256,
        rand() % 256  
    };
    mesh_tris[11] = (struct Tri){                            
        Vector3_create(1, 1, 1), 
        Vector3_create(1, 0, 1), 
        Vector3_create(1, 1, 0),
        rand() % 256,
        rand() % 256,
        rand() % 256  
    };

    // Make center of cube (0, 0, 0).
    struct Vector3 shift = Vector3_create(-0.5, -0.5, -0.5);
    for (int i = 0; i < model_mesh.num_tris; ++i) {
        mesh_tris[i].p1 = Vector3_add(mesh_tris[i].p1, shift);
        mesh_tris[i].p2 = Vector3_add(mesh_tris[i].p2, shift);
        mesh_tris[i].p3 = Vector3_add(mesh_tris[i].p3, shift);
    }

    model_mesh.tris = mesh_tris;

    return Model_create(
        0, 0, 0, 
        0, 0, 0, 
        1, 1, 1, 
        model_mesh
    );
}