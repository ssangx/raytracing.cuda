#ifndef AABB_H
#define AABB_H

#include "ray.h"
#include "float.h"

// avoid too much sanity check
__device__ inline float ffmin(float a, float b) {return a < b ? a : b;}
__device__ inline float ffmax(float a, float b) {return a > b ? a : b;}


/* axis-aligned bounding boxes */

class AABB {
public:

    __device__ AABB() {
        float minNum = FLT_MIN;
        float maxNum = FLT_MAX;
        _min = vec3(maxNum, maxNum, maxNum);
        _max = vec3(minNum, minNum, minNum);
    }

    // bbox of a point
    __device__ AABB(const vec3& p) : _min(p), _max(p) {}

    // regular constructor
    // TODO: add sanity check
    __device__ AABB(const vec3& p1, const vec3& p2) : _min(p1), _max(p2) {}

    __device__ bool hit(const Ray& r, 
                        float t_min, 
                        float t_max) const {
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

    // TODO: get one corner of the aabb 
    __device__ vec3 getCorner(int corner) const { return vec3(0, 0, 0); }

    // TODO: get union of two aabb
    __device__ AABB getUnion(const AABB& aabb) const { return AABB(); }

    __device__ vec3 min() const { return _min; }
    __device__ vec3 max() const { return _max; }

    vec3 _min, _max;
};


// Get union of two aabb, for temporary use
__device__  AABB surrounding_box(AABB box0, AABB box1){
    vec3 small(fmin(box0.min().x(), box1.min().x()),
               fmin(box0.min().y(), box1.min().y()),
               fmin(box0.min().z(), box1.min().z()));
    vec3 big  (fmax(box0.max().x(), box1.max().x()),
               fmax(box0.max().y(), box1.max().y()),
               fmax(box0.max().z(), box1.max().z()));
    return AABB(small, big);
}


#endif  /* AABB_H */