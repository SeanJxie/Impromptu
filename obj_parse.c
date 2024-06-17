#include "obj_parse.h"

inline void split(const char *line, char out[MAX_NUM_SUBSTR][MAX_LEN_SUBSTR], int *out_n) {
    int i = 0; // Line string index.
    int j = 0; // Substring index.
    int k = 0; // Out string array index.

    char ss[MAX_LEN_SUBSTR];
    
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

inline void parse_mtl(const char *file_name, struct Obj_mtl *out, int *out_n) {
    FILE *fp = fopen("models/casa.mtl", "r");

    char line[MAX_LEN_LINE];
    char s[MAX_NUM_SUBSTR][MAX_LEN_SUBSTR];
    int  len_s;

    int mtl_i = 0;

    while (fgets(line, MAX_LEN_LINE, fp)) {
        split(line, s, &len_s);

        if (strcmp(s[0], "newmtl") == 0) {
            strcpy(out[mtl_i].id, s[1]);
        }

        if (strcmp(s[0], "Kd") == 0) {
            out[mtl_i].kd = Vector3_create_point(atof(s[1]), atof(s[2]), atof(s[3]));
            ++mtl_i;
        }
    }

    *out_n = mtl_i;

    fclose(fp);
}