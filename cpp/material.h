#ifndef MATERIAL_H
#define MATERIAL_H

#include "hitable.h"
#include "texture.h"


vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2.0*vec3(drand48(),drand48(),drand48()) - vec3(1,1,1);
    } while (p.squared_length() >= 1.0);
    return p;
}

vec3 reflect(const vec3& v, const vec3& n){
    return v - 2*dot(v, n)*n;
}

float schlick(float cosine, float ref_idx){
    float r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted){
    vec3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1-dt*dt);
    if (discriminant > 0) {
        refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
        return true;
    }
    else 
        return false;
}


class Material{
public: 
    virtual bool scatter(const Ray& r_in, const HitRecord& rec, vec3& attenuation, Ray& scattered) const = 0;
    virtual vec3 emitted(float u, float v, const vec3& p) const { return vec3(0, 0, 0); }
};


class Normal: public Material{
public:
    Normal() {};
    virtual bool scatter(const Ray& r_in, const HitRecord& rec, vec3& attenuation, Ray& scattered) const {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = Ray(rec.p, reflected);
        attenuation = (rec.normal * 0.5) + vec3(0.5, 0.5, 0.5);
        return true;
    }
};


class Lambertian: public Material{
public:
    Lambertian(Texture* a): albedo(a) {}
    virtual bool scatter(const Ray& r_in, const HitRecord& rec, vec3& attenuation, Ray& scattered) const{
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = Ray(rec.p, target-rec.p, r_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }
    Texture* albedo;
};


class Metal: public Material{
public: 
    Metal(const vec3& a, float f): albedo(a) {if(f < 1) fuzz = f; else fuzz = 1;}
    virtual bool scatter(const Ray& r_in, const HitRecord& rec, vec3& attenuation, Ray& scattered) const{
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = Ray(rec.p, reflected + fuzz*random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
    vec3 albedo;
    float fuzz;
};


class Dielectric: public Material{
public: 
    Dielectric(float ri): ref_idx(ri) {}
    virtual bool scatter(const Ray& r_in, const HitRecord& rec, vec3& attenuation, Ray& scattered) const{
        vec3 outward_normal;
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        float ni_over_nt;
        attenuation = vec3(1.0, 1.0, 1.0);
        vec3 refracted;
        float reflect_prob, cosine;
        if(dot(r_in.direction(), rec.normal) > 0){
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx;
            cosine = dot(r_in.direction(), rec.normal) / r_in.direction().length();
            cosine = sqrt(1 - ref_idx*ref_idx*(1-cosine*cosine));
        }else{
            outward_normal = rec.normal;
            ni_over_nt = 1.0 / ref_idx;
            cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }
        
        if(refract(r_in.direction(), outward_normal, ni_over_nt, refracted)){
            reflect_prob = schlick(cosine, ref_idx);
        }else{
            scattered = Ray(rec.p, reflected);
            reflect_prob = 1.0;
        }

        if(drand48() < reflect_prob){
            scattered = Ray(rec.p, reflected);
        }else{
            scattered = Ray(rec.p, refracted);
        }
        return true;
    }
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


class Isotropic: public Material{
public: 
    Isotropic(Texture* a): albedo(a) {}
    virtual bool scatter(const Ray& r_in, const HitRecord& rec, vec3& attenuation, Ray& scattered) const{
        scattered = Ray(rec.p, random_in_unit_sphere());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

    Texture* albedo;
};
#endif