#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hitable/hitable.h"


// Triangle primitive

class Triangle: public Hitable {
public:
    __device__ Triangle() : EPSILON(0.00001) {}
    __device__ Triangle(vec3 vs[3], Material* mat, const bool cull=false) : 
                        EPSILON(0.000001) {
        for(int i = 0; i < 3; i++) {
            vertices[i] = vs[i];
        }
        material = mat;
        backCulling = cull;
    };

    __device__ virtual bool hit(const Ray& r, 
                                float t_min, 
                                float t_max, 
                                HitRecord& rec) const;

    __device__ virtual bool bounding_box(float t0,
                                         float t1,
                                         AABB& box) const;

    const float EPSILON;

    vec3 vertices[3];
    bool backCulling;
    Material* material;
};


__device__ bool Triangle::hit(const Ray& r, 
                              float t_min, 
                              float t_max, 
                              HitRecord& rec) const {
    vec3 vertex0 = vertices[0];
    vec3 vertex1 = vertices[1];
    vec3 vertex2 = vertices[2];

    vec3 edge1, edge2, h, s, q;
    float a, f, u, v;

    edge1 = vertex1 - vertex0;
    edge2 = vertex2 - vertex0;
    h = cross(r.direction(), edge2);
    a = dot(edge1, h);

    if (a < EPSILON && backCulling)
        return false;

    // Almost parallel to the triangle
    if (a > - EPSILON && a < EPSILON)
        return false;

    f = 1.0 / a;
    s = r.origin() - vertex0;
    u = f * dot(s, h);

    if (u < 0.0 || u > 1.0)
        return false;

    q = cross(s, edge1);
    v = f * dot(r.direction(), q);
    if (v < 0.0 || u + v > 1.0)
        return false;

    // Compute final t
    float t = f * dot(edge2, q);

    rec.t = t;
    rec.p = r.point_at_t(rec.t);
    rec.normal = unit_vector(cross(edge1, edge2));
    rec.mat_ptr = material;

    return true;
}

__device__ bool Triangle::bounding_box(float t0,
                                       float t1,
                                       AABB& box) const {
    return true;
}


#endif  /* TRIANGLE_H */