#ifndef MATERIAL_H
#define MATERIAL_H

#include "hitable.h"
#include "texture.h"


class Material{
public: 
    virtual bool scatter(const Ray& r_in, const HitRecord& rec, vec3& attenuation, Ray& scattered) const = 0;
    virtual vec3 emitted(float u, float v, const vec3& p) const { return vec3(0, 0, 0); }
};


class Normal: public Material{
public:
    Normal() {};
    virtual bool scatter(const Ray& r_in, const HitRecord& rec, vec3& attenuation, Ray& scattered) const;
};


class Lambertian: public Material{
public:
    Lambertian(Texture* a): albedo(a) {}
    virtual bool scatter(const Ray& r_in, const HitRecord& rec, vec3& attenuation, Ray& scattered) const;
    Texture* albedo;
};


class Metal: public Material{
public: 
    Metal(const vec3& a, float f): albedo(a) {if(f < 1) fuzz = f; else fuzz = 1;}
    virtual bool scatter(const Ray& r_in, const HitRecord& rec, vec3& attenuation, Ray& scattered) const;
    vec3 albedo;
    float fuzz;
};


class Dielectric: public Material{
public: 
    Dielectric(float ri): ref_idx(ri) {}
    virtual bool scatter(const Ray& r_in, const HitRecord& rec, vec3& attenuation, Ray& scattered) const;
    float ref_idx;
};


class DiffuseLight: public Material{
public:
    DiffuseLight(Texture* texture): emit(texture) {}
    virtual bool scatter(const Ray& r_in, const HitRecord& rec, vec3& attenuation, Ray& scattered) const{
        return false;
    }
    virtual vec3 emitted(float u, float v, const vec3& p) const{
        return emit->value(u, v, p);
    }

    Texture* emit;
};


//class Isotropic: public Material{
//public:
//    Isotropic(Texture* a): albedo(a) {}
//    virtual bool scatter(const Ray& r_in, const HitRecord& rec, vec3& attenuation, Ray& scattered) const{
//        scattered = Ray(rec.p, random_in_unit_sphere());
//        attenuation = albedo->value(rec.u, rec.v, rec.p);
//        return true;
//    }
//
//    Texture* albedo;
//};


#endif