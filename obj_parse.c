#include "obj_parse.h"

inline void split_space(const char *line, char out[MAX_NSS][MAX_LSS], int *out_n) {
    int i = 0; // Line string index.
    int j = 0; // Substring index.
    int k = 0; // Out string array index.

    char ss[MAX_LSS];
    
    while (line[i] != '\0') {
        
        // Skip any whitespaces.
        while (line[i] == ' ') ++i;

        // Read substring until whitespace or end of string.
        j = 0;
        while (line[i] != ' ' && line[i] != '\0') {
            ss[j++] = line[i++];
        }

        // Terminate substring.
        ss[j] = '\0';

        // Write to out.
        strcpy(out[k++], ss);
    }

    *out_n = k;
}

inline void split_slash(const char *line, char out[MAX_NSS][MAX_LSS], int *out_n) {
    int i = 0;
    int j = 0;
    int k = 0;

    char ss[MAX_LSS];
    
    while (line[i] != '\0') {
        
        // Skip a single slash.
        if (line[i] == '/') ++i;

        // Read substring until slash or end of string.
        j = 0;
        while (line[i] != '/' && line[i] != '\0') {
            ss[j++] = line[i++];
        }

        ss[j] = '\0';
        strcpy(out[k++], ss);
    }

    *out_n = k;
}

inline struct Tri *parse_obj(const char *file_name, int *out_n) {
    int nv  = 0; // Vertex.
    int nvt = 0; // Vertex texture.
    int nvn = 0; // Vertex normal.
    int nf  = 0; // Face (triangle).

    char line[MAX_LL];
    char space_split_arr[MAX_NSS][MAX_LSS];
    char slash_split_arr[MAX_NSS][MAX_LSS];
    int  len_space_split_arr;
    int  len_slash_split_arr;
    char *keyword;

    FILE *fp = fopen(file_name, "r");

    // Count vertex data and face elements.
    while (fgets(line, MAX_LL, fp)) {
        split_space(line, space_split_arr, &len_space_split_arr);
        keyword = space_split_arr[0];

        if      (strcmp("v" , keyword) == 0) ++nv;
        else if (strcmp("vt", keyword) == 0) ++nvt;
        else if (strcmp("vn", keyword) == 0) ++nvn;
        else if (strcmp("f" , keyword) == 0) ++nf;
    }

    fseek(fp, 0, SEEK_SET);
    
    // Vertex position, texture coordinate, and normal storage.
    struct Vector3 *v  = malloc(sizeof(struct Vector3) * nv );
    struct Vector3 *vt = malloc(sizeof(struct Vector3) * nvt);
    struct Vector3 *vn = malloc(sizeof(struct Vector3) * nvn);

    // Mesh to return.
    struct Tri *f = malloc(sizeof(struct Tri) * nf);

    // Storage indeces.
    int vi  = 0; // Vertex.
    int vti = 0; // Vertex texture.
    int vni = 0; // Vertex normal.
    int fi  = 0; // Face (triangle).

    // References.
    int vref;
    int vtref;
    int vnref;

    // Material.
    // struct Obj_mtl curr_mtl;
    // struct Obj_mtl mtl_lib[MAX_NM];
    //int            num_mtl;

    // Populate storage.
    while (fgets(line, MAX_LL, fp)) {
        split_space(line, space_split_arr, &len_space_split_arr);
        keyword = space_split_arr[0];

        if (strcmp("v", keyword) == 0) {
            v[vi++] = (struct Vector3){
                atof(space_split_arr[1]), 
                atof(space_split_arr[2]), 
                atof(space_split_arr[3]),
                1,
            };
        } else if (strcmp("vt", keyword) == 0) {
            vt[vti++] = (struct Vector3){
                atof(space_split_arr[1]), 
                atof(space_split_arr[2]), 
                0, // Unused z.
                0  // Unused w. 
            };
        } else if (strcmp("vn", keyword) == 0) {
            vn[vni++] = (struct Vector3){
                atof(space_split_arr[1]), 
                atof(space_split_arr[2]), 
                atof(space_split_arr[3]),
                0
            };
        } else if (strcmp("f", keyword) == 0) {
            // Assume a triangular face (i.e. 3 vertices).
            struct Tri tmpt;
            struct Vertex tmpv;
            for (int i = 1; i < 4; ++i) {
                split_slash(space_split_arr[i], slash_split_arr, &len_slash_split_arr);
            
                vref = atoi(slash_split_arr[0]); // Required vertex reference.
                tmpv.pos = v[vref - 1];

                if (len_slash_split_arr >= 2 && strcmp("", slash_split_arr[1]) != 0) { // Optional texture coordinate reference.
                    vtref = atoi(slash_split_arr[1]);
                    tmpv.tex_u  = vt[vtref - 1].x;
                    tmpv.tex_v  = vt[vtref - 1].y;
                } 

                if (len_slash_split_arr == 3 && strcmp("", slash_split_arr[2]) != 0) { // Optional vertex normal reference.
                    vnref = atoi(slash_split_arr[2]);
                    tmpv.normal = vn[vnref - 1];
                }

                switch (i) {
                    case 1: tmpt.v0 = tmpv; break;
                    case 2: tmpt.v1 = tmpv; break;
                    case 3: tmpt.v2 = tmpv; break;
                } 
            }

            tmpt.r = rand() % 256;
            tmpt.g = rand() % 256;
            tmpt.b = rand() % 256;

            f[fi++] = tmpt;
        }
    }

    fclose(fp);
    free(v);
    free(vt);
    free(vn);
    
    *out_n = nf;
    return f;
}

// inline void parse_mtl(const char *file_name, struct Obj_mtl *out, int *out_n) {
//     FILE *fp = fopen("models/casa.mtl", "r");

//     char line[MAX_LEN_LINE];
//     char s[MAX_NUM_SUBSTR][MAX_LEN_SUBSTR];
//     int  len_s;

//     int mtl_i = 0;

//     while (fgets(line, MAX_LEN_LINE, fp)) {
//         split_space(line, s, &len_s);

//         if (strcmp(s[0], "newmtl") == 0) {
//             strcpy(out[mtl_i].id, s[1]);
//         }

//         if (strcmp(s[0], "Kd") == 0) {
//             out[mtl_i].kd = Vector3_create_point(atof(s[1]), atof(s[2]), atof(s[3]));
//             ++mtl_i;
//         }
//     }

//     *out_n = mtl_i;

//     fclose(fp);
// }