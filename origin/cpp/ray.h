#ifndef RAYH
#define RAYH

#include "vec3.h"


class Ray{
public:
    Ray() {}
    Ray(const vec3& o, const vec3& d, float t = 0.0) {_origin = o; _direction = d; _time = t;}

    vec3 origin() const {return _origin;}
    vec3 direction() const {return _direction;}
    float time() const {return _time;}
    vec3 point_at_t(float t) const {return _origin + t * _direction;}
    
    // origin and direction
    vec3 _origin;
    vec3 _direction;
    // the time the ray exist at 
    float _time;
};


#endif