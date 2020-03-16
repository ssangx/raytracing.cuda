#include "sphere.h"


bool Sphere::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const{
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    // b^2 - 4ac
    float discriminant = b*b - a*c;
    if(discriminant > 0){
        // root
        float sol = (-b - sqrt(discriminant)) / a;
        if(sol < t_max && sol > t_min){
            rec.t = sol;
            rec.p = r.point_at_t(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
        sol = (-b + sqrt(discriminant)) / a;
        if(sol < t_max && sol > t_min){
            rec.t = sol;
            rec.p = r.point_at_t(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}


bool Sphere::bounding_box(AABB& box) const{
    box = AABB(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
    return true;
}