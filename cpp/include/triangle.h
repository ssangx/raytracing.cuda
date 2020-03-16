#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hitable.h"
#include <iostream> 

// Triangle primitive

class Triangle: public Hitable{
public:
    Triangle() : EPSILON(0.0000001) {}
    Triangle(vec3 vs[3], vec3 n[3], Material* mat, const bool cull=false);

    virtual bool hit(const Ray& r,  float t_min,  float t_max,  HitRecord& rec) const;
    virtual bool bounding_box(AABB& box) const;

    const float EPSILON;

    vec3 vertices[3];
    vec3 normals[3];

    bool backCulling;
    Material* material;
};


#endif /* TRIANGLE_H */