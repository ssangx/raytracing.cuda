#ifndef SPHERE_H
#define SPHERE_H

#include "hitable.h"


class Sphere: public Hitable{
public:
    Sphere() {}
    Sphere(vec3 c, float r, Material* m): center(c), radius(r), mat_ptr(m) {};
    virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
    virtual bool bounding_box(AABB& box) const;
    vec3 center;
    float radius;
    Material *mat_ptr;
};


#endif