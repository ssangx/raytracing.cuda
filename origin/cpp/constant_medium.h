#ifndef CONSTANTMEDIUMH
#define CONSTANTMEDIUMH

#include "hitable.h"
#include "material.h"


class ConstantMedium: public Hitable{
public:
    ConstantMedium(Hitable* b, float d, Texture* a): boundary(b), density(d){
        phase_function = new Isotropic(a);
    }
    virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
    virtual bool bounding_box(float t0, float t1, AABB& box) const{
        return boundary->bounding_box(t0, t1, box);
    }

    float density;
    Hitable* boundary;
    Material* phase_function;
};

bool ConstantMedium::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const{
    bool db = (drand48() < 0.00001);
    db = false;
    HitRecord rec1, rec2;
    if (boundary->hit(r, -FLT_MAX, FLT_MAX, rec1)) { 
        if (boundary->hit(r, rec1.t+0.0001, FLT_MAX, rec2)) {
    if (db) std::cerr << "\nt0 t1 " << rec1.t << " " << rec2.t << "\n";
            if (rec1.t < t_min) rec1.t = t_min;
            if (rec2.t > t_max) rec2.t = t_max;
            if (rec1.t >= rec2.t) return false;
            if (rec1.t < 0) rec1.t = 0;
            float distance_inside_boundary = (rec2.t - rec1.t) * r.direction().length();
            float hit_distance = - (1/density) * log(drand48()); 
            if (hit_distance < distance_inside_boundary) {
            if (db) std::cerr << "hit_distance = " <<  hit_distance << "\n";
                rec.t = rec1.t + hit_distance / r.direction().length(); 
            if (db) std::cerr << "rec.t = " <<  rec.t << "\n";
                rec.p = r.point_at_t(rec.t);
            if (db) std::cerr << "rec.p = " <<  rec.p << "\n";
                rec.normal = vec3(1, 0, 0);  // arbitrary
                rec.mat_ptr = phase_function;
                return true;
            }
        }
    }
    return false;
}

#endif