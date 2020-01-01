#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"


/* Camera */


__device__ vec3 random_in_unit_disk(curandState* state){
    vec3 p;
    do{
        // sample from [-1.0, 1.0]
        p = 2.0f * vec3(curand_uniform(state), curand_uniform(state), 0) - vec3(1, 1, 0);
    } while(dot(p, p) >= 1.0f);
    return p;
}


class Camera {
public: 
    __device__ Camera(vec3 lookfrom, 
                      vec3 lookat, 
                      vec3 vup,
                      float vfov, 
                      float aspect, 
                      float aperture,   
                      float focus_dist){
        lens_radius = aperture / 2.0f;
        float theta = vfov * M_PI / 180.0f;
        float half_height = tan(theta/2.0f);
        float half_width  = half_height * aspect;

        origin = lookfrom;
        z = unit_vector(lookfrom - lookat);
        x = unit_vector(cross(vup, z));
        y = cross(z, x);
        
        lower_left_corner = origin - half_width*focus_dist*x - half_height*focus_dist*y - focus_dist*z;
        horizontal = 2.0f * half_width  * focus_dist * x;
        vertical   = 2.0f * half_height * focus_dist * y;
    }

    __device__ Ray get_ray(float s, float t, curandState* state) {
        vec3 rd = lens_radius * random_in_unit_disk(state);
        vec3 offset = x * rd.x() + y * rd.y();
        return Ray(origin + offset, lower_left_corner + s*horizontal + t*vertical - origin - offset, 0.0f);
    }

    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 origin;
    vec3 x, y, z;

    float lens_radius;
};


class MotionCamera: public Camera{
public: 
    __device__ MotionCamera(vec3 lookfrom, 
                            vec3 lookat, 
                            vec3 vup,
                            float vfov, 
                            float aspect, 
                            float aperture,   
                            float focus_dist,
                            float t0,
                            float t1) : Camera(lookfrom, lookat, vup, vfov, 
                                                aspect, aperture, focus_dist){

        time0 = t0;
        time1 = t1;
    }

    __device__ Ray get_ray(float s, float t, curandState* state) {
        vec3 rd = lens_radius * random_in_unit_disk(state);
        vec3 offset = x * rd.x() + y * rd.y();
        float time = time0 + curand_uniform(state) * (time1 - time0);
        return Ray(origin + offset, lower_left_corner + s*horizontal + t*vertical - origin - offset, time);
    }

    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 origin;
    vec3 x, y, z;

    float lens_radius;
    float time0;
    float time1;
};

 
#endif // CAMERA_H