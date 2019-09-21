#ifndef CONSTANTMEDIUMH
#define CONSTANTMEDIUMH

#include "hitable.h"
#include "material.h"


class ConstantMedium: public Hitable{
public:
    __device__ ConstantMedium(Hitable* b, float d, Texture* a): boundary(b), density(d){
        phase_function = new Isotropic(a);
    }
    __device__ virtual bool hit(const Ray& r, 
                                float t_min, 
                                float t_max, 
                                HitRecord& rec) const;

    __device__ virtual bool bounding_box(float t0, float t1, AABB& box) const{
        return boundary->bounding_box(t0, t1, box);
    }

    float density;
    Hitable* boundary;
    Material* phase_function;
};


__device__ bool ConstantMedium::hit(const Ray& r, 
                                    float t_min, 
                                    float t_max, 
                                    HitRecord& rec) const{
    HitRecord rec1, rec2;
    if (boundary->hit(r, -FLT_MAX, FLT_MAX, rec1)) { 
        if (boundary->hit(r, rec1.t+0.0001, FLT_MAX, rec2)) {
            if (rec1.t < t_min) rec1.t = t_min;
            if (rec2.t > t_max) rec2.t = t_max;

            if (rec1.t >= rec2.t) return false;
            if (rec1.t < 0) rec1.t = 0;

            float distance_inside_boundary = (rec2.t - rec1.t) * r.direction().length();
            float hit_distance = - (1/density); 

            if (hit_distance < distance_inside_boundary) {
                rec.t = rec1.t + hit_distance / r.direction().length(); 
                rec.p = r.point_at_t(rec.t);
                rec.normal = vec3(1, 0, 0);
                rec.mat_ptr = phase_function;
                return true;
            }
        }
    }
    return false;
}

#endif