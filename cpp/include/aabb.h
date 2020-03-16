#ifndef AABB_H
#define AABB_H

#include "ray.h"


class AABB {
public:
    AABB() {}
    AABB(const vec3& a, const vec3& b) {_min = a; _max = b;}

    bool hit(const Ray& r, float t_min, float t_max) const;

    vec3 min() const {return _min;}
    vec3 max() const {return _max;}

    vec3 _min, _max;
};


AABB surrounding_box(AABB box0, AABB box1);

#endif