#ifndef RAY_H
#define RAY_H

#include "vec3.h"

/* Basic ray definition */

class Ray {
public:
    __device__ Ray() : _origin(vec3(0.f)), _direction(vec3(0.f)), _time(0.f) {}

    __device__ Ray(const vec3& o, const vec3& d, float t=0.f) : _origin(o), _direction(d), _time(t) {}

    __device__ float time() const { return _time; }
    __device__ vec3 origin() const { return _origin; }
    __device__ vec3 direction() const { return _direction; }
    
    // t is parameter, not time t
    __device__ vec3 point_at_t(float t) const { return _origin + t * _direction; }

    float _time;
    vec3 _origin;
    vec3 _direction;
};


#endif /* RAY_H */