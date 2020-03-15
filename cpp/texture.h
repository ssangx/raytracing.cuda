#ifndef TEXTURE_H
#define TEXTURE_H

#include "vec3.h"
#include "perlin.h"

class Texture {
public:
    virtual vec3 value(float u, float v, const vec3& p) const = 0;
};


class ConstantTexture: public Texture {
public: 
    ConstantTexture() {}
    ConstantTexture(vec3 c): color(c) {};

    virtual vec3 value(float u, float v, const vec3& p) const {
        return color;
    }

    vec3 color;
};


class CheckerTexture: public Texture {
public:
    CheckerTexture() {}
    CheckerTexture(Texture* t0, Texture* t1): even(t0), odd(t1) {}

    virtual vec3 value(float u, float v, const vec3& p) const {
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


class NoiseTexture: public Texture{
public:
    NoiseTexture() {}
    NoiseTexture(float sc): scale(sc) {}
    virtual vec3 value(float u, float v, const vec3& p) const{
        // return vec3(1, 1, 1) * noise.noise(scale * p);
        // return vec3(1, 1, 1) * 0.5 * (1 + noise.turb(scale * p));
        // return vec3(1, 1, 1) * noise.turb(scale * p);
        return vec3(1, 1, 1) * 0.5 * (1 + sin(scale*p.z() + 10 * noise.turb(p)));
    }
    Perlin noise;
    float scale;
};


void get_sphere_uv(const vec3& p, float& u, float& v){
    float phi = atan2(p.z(), p.x());
    float theta = asin(p.z());
    u = 1 - (phi + M_PI) / (2 * M_PI);
    v = (theta + M_PI/2) / M_PI;
}


class ImageTexture: public Texture{
public:
    ImageTexture() {}
    ImageTexture(unsigned char* pixels, int A, int B): data(pixels), nx(A), ny(B) {}
    virtual vec3 value(float u, float v, const vec3& p) const;

    unsigned char* data;
    int nx, ny;
};


vec3 ImageTexture::value(float u, float v, const vec3& p) const{
    int i = (u) * nx;
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