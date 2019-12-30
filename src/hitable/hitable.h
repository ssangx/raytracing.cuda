#ifndef HITABLE_H
#define HITABLE_H

#include "core/ray.h"
#include "core/aabb.h"


class Material;

struct HitRecord{
    float t;
    float u;
    float v;
    vec3  p;
    vec3  normal;
    Material* mat_ptr;
};


// The abstract class for all object that a ray can hit
// two functions, hit and bounding_box 

class Hitable {
public:
    __device__ virtual bool hit(const Ray& r, 
                                float t_min, 
                                float t_max, 
                                HitRecord& rec) const = 0;

    __device__ virtual bool bounding_box(float t0,
                                         float t1,
                                         AABB& box) const = 0;
};


class FlipNormals: public Hitable{
public:
    __device__ FlipNormals(Hitable* p): ptr(p) {}

    __device__ virtual bool hit(const Ray& r, 
                                float t_min, 
                                float t_max, 
                                HitRecord& rec) const{
        if(ptr->hit(r, t_min, t_max, rec)){
            rec.normal = -rec.normal;
            return true;
        }else{
            return false;
        }
    }

    __device__ virtual bool bounding_box(float t0, 
                                         float t1, 
                                         AABB& box) const{
        return ptr->bounding_box(t0, t1, box);
    }

    Hitable* ptr;
};


#endif  /* HITABLE_H */