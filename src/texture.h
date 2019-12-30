#ifndef TEXTUREH
#define TEXTUREH

#include "core/vec3.h"


class Texture {
public:
    __device__ virtual vec3 value(float u, float v, const vec3& p) const = 0;
};


class ConstantTexture: public Texture {
public: 
    __device__ ConstantTexture() {}
    __device__ ConstantTexture(vec3 c): color(c) {};

    __device__ virtual vec3 value(float u, float v, const vec3& p) const {
        return color;
    }

    vec3 color;
};


class CheckerTexture: public Texture {
public:
    __device__ CheckerTexture() {}
    __device__ CheckerTexture(Texture* t0, Texture* t1): even(t0), odd(t1) {}

    __device__ virtual vec3 value(float u, float v, const vec3& p) const {
        float sines = sin(10*p.x()) * sin(10*p.y()) * sin(10*p.z());
        if(sines < 0){
            return odd->value(u, v, p);
        }else{
            return even->value(u, v, p);
        }
    }

    Texture* odd;
    Texture* even;
};


__device__ void get_sphere_uv(const vec3& p, float& u, float& v){
    float phi = atan2(p.z(), p.x());
    float theta = asin(p.z());
    u = 1 - (phi + M_PI) / (2 * M_PI);
    v = (theta + M_PI/2) / M_PI;
}


class ImageTexture: public Texture{
public:
    __device__ ImageTexture() {}
    __device__ ImageTexture(unsigned char* pixels, int A, int B): data(pixels), nx(A), ny(B) {}
    __device__ virtual vec3 value(float u, float v, const vec3& p) const;

    unsigned char* data;
    int nx, ny;
};


__device__ vec3 ImageTexture::value(float u, float v, const vec3& p) const{
    int i = u * nx;
    int j = (1-v) * ny - 0.001;
    if(i < 0) i = 0;
    if(j < 0) j = 0;
    if(i > nx - 1) i = nx - 1;
    if(j > ny - 1) j = ny - 1;
    
    float r = int(data[3*i + 3*nx*j  ]) / 255.0;
    float g = int(data[3*i + 3*nx*j+1]) / 255.0;
    float b = int(data[3*i + 3*nx*j+2]) / 255.0;
    return vec3(r, g, b);
}

#endif