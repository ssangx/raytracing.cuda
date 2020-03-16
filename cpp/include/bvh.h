#ifndef BHVNODE_H
#define BHVNODE_H

#include "hitable.h"


class BVHNode: public Hitable{
public:
    BVHNode() {}
    BVHNode(Hitable **l, int n);

    virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
    virtual bool bounding_box(AABB& b) const;
    
    Hitable* left;
    Hitable* right;
    AABB box;
};

#endif