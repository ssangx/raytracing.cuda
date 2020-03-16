#ifndef BOX_H
#define BOX_H

#include "bvh.h"
#include "material.h"
#include "rectangle.h"


class Box: public Hitable{
public:
    Box(const vec3& p0, const vec3& p1, Material* ptr);

    virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
    virtual bool bounding_box(AABB& box) const {
        box = AABB(p_min, p_max);
        return true;
    }

    vec3 p_min, p_max;
    Hitable* list_ptr;
};


#endif