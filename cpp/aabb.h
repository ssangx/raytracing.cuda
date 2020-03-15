#ifndef AABB_H
#define AABB_H

#include "ray.h"


inline float ffmin(float a, float b) {return a < b ? a : b;}
inline float ffmax(float a, float b) {return a > b ? a : b;}

class AABB {
public:
    AABB() {}
    AABB(const vec3& a, const vec3& b) {_min = a; _max = b;}

    vec3 min() const {return _min;}
    vec3 max() const {return _max;}
    
    bool hit(const Ray& r, float t_min, float t_max) const {
        for(int a = 0; a < 3; a++) {
            float t0 = ffmin((_min[a] - r.origin()[a]) / r.direction()[a], 
                             (_max[a] - r.origin()[a]) / r.direction()[a]);
            float t1 = ffmax((_min[a] - r.origin()[a]) / r.direction()[a], 
                             (_max[a] - r.origin()[a]) / r.direction()[a]);
            t_min = ffmax(t0, t_min);
            t_max = ffmin(t1, t_max);
            if(t_max <= t_min) return false;
        }
        return true;
    }

    vec3 _min, _max;
};


AABB surrounding_box(AABB box0, AABB box1) {
    vec3 small(fmin(box0.min().x(), box1.min().x()),
               fmin(box0.min().y(), box1.min().y()),
               fmin(box0.min().z(), box1.min().z()));
    vec3 big  (fmax(box0.max().x(), box1.max().x()),
               fmax(box0.max().y(), box1.max().y()),
               fmax(box0.max().z(), box1.max().z()));
    return AABB(small, big);
}

#endif