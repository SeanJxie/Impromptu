#include "plane.h"

inline int Plane_intersect_line(struct Plane p, struct Vector3 start, struct Vector3 end, struct Vector3 *intersect) {
    struct Vector3 line_dir = Vector3_normalize(Vector3_sub(end, start));

    float line_dot_norm = Vector3_dot(line_dir, p.n);

    if (line_dot_norm == 0) {
        // If the line and plane are parralel (this
        // includes the case where the line is contained in
        // the plane), report no intersection.
        return 0;
    }

    // Line parameter at which the intersection occurs.
    float t_start     = 0;
    float t_end       = Vector3_dot(Vector3_sub(end, start), line_dir);
    float t_intersect = Vector3_dot(Vector3_sub(p.p0, start), p.n) / line_dot_norm;

    // Ensure the intersection falls within the line start and end.
    if (MIN(t_start, t_end) < t_intersect && t_intersect < MAX(t_start, t_end)) {
         *intersect = Vector3_add(start, Vector3_smul(line_dir, t_intersect));

        return 1;
    }
   
    return 0;
}