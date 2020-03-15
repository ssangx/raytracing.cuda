#ifndef SPHERE_H
#define SPHERE_H

#include "hitable.h"


class Sphere: public Hitable{
public:
    Sphere() {}
    Sphere(vec3 c, float r, Material* m): center(c), radius(r), mat_ptr(m) {};
    virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
    virtual bool bounding_box(float t0, float t1, AABB& box) const;
    vec3 center;
    float radius;
    Material *mat_ptr;
};

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

bool Sphere::bounding_box(float t0, float t1, AABB& box) const{
    box = AABB(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
    return true;
}


class MovingSphere: public Hitable{
public:
    MovingSphere() {}
    MovingSphere(vec3 cen0, vec3 cen1, float t0, float t1, float r, Material *m): 
                center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m) {};
    
    virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
    virtual bool bounding_box(float t0, float t1, AABB& box) const;
    vec3 center(float time) const;

    // centers at time0 and time1
    vec3 center0, center1;
    float time0, time1;
    float radius;
    Material *mat_ptr;
};

vec3 MovingSphere::center(float time) const{
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool MovingSphere::bounding_box(float t0, float t1, AABB& box) const {
        AABB box0(center(t0) - vec3(radius, radius, radius), center(t0) + vec3(radius, radius, radius));
        AABB box1(center(t1) - vec3(radius, radius, radius), center(t1) + vec3(radius, radius, radius));
        box = surrounding_box(box0, box1);
        return true;
}

bool MovingSphere::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const{
    vec3 oc = r.origin() - center(r.time());
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
            rec.normal = (rec.p - center(r.time())) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
        sol = (-b + sqrt(discriminant)) / a;
        if(sol < t_max && sol > t_min){
            rec.t = sol;
            rec.p = r.point_at_t(rec.t);
            rec.normal = (rec.p - center(r.time())) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

#endif