#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hitable.h"
#include <iostream> 

// Triangle primitive

class Triangle: public Hitable{
public:
    Triangle() : EPSILON(0.00001) {}

    Triangle(vec3 vs[3],
             Material* mat,
             const bool cull=false) : material(mat), backCulling(cull), EPSILON(0.00001) {
        for(int i = 0; i < 3; i++) {
            vertices[i] = vs[i];
        }
    };

    virtual bool hit(const Ray& r, 
                     float t_min, 
                     float t_max, 
                     HitRecord& rec) const;

    virtual bool bounding_box(float t0,
                              float t1,
                              AABB& box) const;

    const float EPSILON;

    vec3 vertices[3];
    bool backCulling;
    Material* material;
};


bool Triangle::hit(const Ray& r, 
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



bool Triangle::bounding_box(float t0,
                            float t1,
                            AABB& bbox) const {
    // float minX = min(vertices[0][0], min(vertices[1][0], vertices[2][0]));
    // float minY = min(vertices[0][1], min(vertices[1][1], vertices[2][1]));
    // float minZ = min(vertices[0][2], min(vertices[1][2], vertices[2][2]));

    // float maxX = max(vertices[0][0], max(vertices[1][0], vertices[2][0]));
    // float maxY = max(vertices[0][1], max(vertices[1][1], vertices[2][1]));
    // float maxZ = max(vertices[0][2], max(vertices[1][2], vertices[2][2]));

    // bbox = AABB(vec3(minX, minY, minZ), vec3(maxX, maxY, maxZ));
    return true;
}


#endif /* TRIANGLE_H */