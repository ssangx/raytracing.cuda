#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "hitable.h"
#include "triangle.h"


/**
 * Rectangle along XY axes
 */
class RectangleXY: public Hitable
{
public:
    RectangleXY() {};
    RectangleXY(float _x0, float _x1, float _y0, float _y1, float _k, Material* mat):
        x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mat_ptr(mat) {};
    
    virtual bool hit(const Ray& r, float t0, float t1, HitRecord& rec) const;
    virtual bool bounding_box(AABB& box) const{
        box = AABB(vec3(x0, y0, k-0.0001), vec3(x1, y1, k+0.0001));
        return true;
    }

    float x0, x1, y0, y1, k;
    Material * mat_ptr;
};


/**
 * Rectangle along XZ axes
 */
class RectangleXZ: public Hitable
{
public:
    RectangleXZ() {};
    RectangleXZ(float _x0, float _x1, float _z0, float _z1, float _k, Material* mat):
        x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mat_ptr(mat) {};
    
    virtual bool hit(const Ray& r, float t0, float t1, HitRecord& rec) const;
    virtual bool bounding_box(AABB& box) const{
        box = AABB(vec3(x0, k-0.0001, z0), vec3(x1, k+0.0001, z1));
        return true;
    }

    float x0, x1, z0, z1, k;
    Material * mat_ptr;
};


/**
 * Rectangle along YZ axes
 */
class RectangleYZ: public Hitable
{
public:
    RectangleYZ() {};
    RectangleYZ(float _y0, float _y1, float _z0, float _z1, float _k, Material* mat):
        y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mat_ptr(mat) {};
    
    virtual bool hit(const Ray& r, float t0, float t1, HitRecord& rec) const;
    virtual bool bounding_box(AABB& box) const{
        box = AABB(vec3(k-0.0001, y0, z0), vec3(k+0.0001, y1, z1));
        return true;
    }

    float y0, y1, z0, z1, k;
    Material * mat_ptr;
};



/**
 * Rectangle defined by triangles
 */
class Rectangle: public Hitable {
};


#endif