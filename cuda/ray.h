#ifndef RAYH
#define RAYH

#include "vec3.h"


class Ray{
public:
    __device__ Ray() {}
    __device__ Ray(const vec3& o, const vec3& d, float t=0.0){
        __origin = o; __direction = d, __time = t;
    }

    __device__ float time() const {return __time;}
    __device__ vec3 origin() const {return __origin;}
    __device__ vec3 direction() const {return __direction;}
    
    __device__ vec3 point_at_t(float t) const {return __origin + t*__direction;}

    // origin and direction
    vec3 __origin;
    vec3 __direction;
    
    // the time the ray exist at 
    float __time;
};


#endif 