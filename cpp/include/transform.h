#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "hitable.h"
#include <float.h>

/**
 * Tanslate
 */
class Translate: public Hitable{
public:
    Translate(Hitable* p, const vec3& displacement): ptr(p), offset(displacement) {}

    virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
    virtual bool bounding_box(AABB& box) const;

    Hitable* ptr;
    vec3 offset;
};


/**
 * Rotation
 */
class Rotate: public Hitable{
public:
    Rotate(Hitable* p, float angle);
    virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
    virtual bool bounding_box(AABB& box) const{
        box = bbox;
        return hasbox;
    }

    float sin_theta;
    float cos_theta;
    bool hasbox;
    AABB bbox;
    Hitable* ptr;
};


#endif