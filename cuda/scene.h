#include <curand.h>
#include <curand_kernel.h>

#include "ray.h"
#include "bvh.h"
#include "box.h"
#include "vec3.h"
#include "float.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "rectangle.h"
#include "transform.h"
#include "hitable_list.h"
#include "constant_medium.h"


__device__ float rand(curandState *state){
    return float(curand_uniform(state));
}


__device__ void random_scene(Hitable **list, 
                             Hitable **world, 
                             curandState *state){
    Texture *checker = new CheckerTexture(new ConstantTexture(vec3(0.2, 0.3, 0.1)),
                                          new ConstantTexture(vec3(0.9, 0.9, 0.9)));
    list[0] = new Sphere(vec3(0, -1000.0, -1), 1000, new Lambertian(checker));
    int i = 1;
    for(int a = -11; a < 11; a++) {
        for(int b = -11; b < 11; b++) {
            float choose_mat = rand(state);
            vec3 center(a + 0.9 * rand(state), 0.2, b + 0.9 * rand(state));
            if(choose_mat < 0.8f) {
                // list[i++] = new MovingSphere(center, center+vec3(0, 0.5*rand(state), 0), 0.0, 1.0, 0.2,
                //                              new Lambertian(new ConstantTexture(vec3(rand(state), rand(state), rand(state)))));
                continue;
            }
            else if(choose_mat < 0.95f) {
                list[i++] = new Sphere(center, 0.2,
                                       new Metal(vec3(0.5f*(1.0f+rand(state)), 
                                                      0.5f*(1.0f+rand(state)), 
                                                      0.5f*(1.0f+rand(state))), 
                                                    0.5f*rand(state)));
            }
            else {
                list[i++] = new Sphere(center, 0.2, new Dielectric(rand(state)*2));
            }
        }
    }
    list[i++] = new Sphere(vec3( 0, 1, 0), 1.0, new Dielectric(1.5));
    list[i++] = new Sphere(vec3(-4, 1, 0), 1.0, new Lambertian(checker));
    list[i++] = new Sphere(vec3( 4, 1, 0), 1.0, new Metal(vec3(0.7, 0.6, 0.5), 0.0));
    *world = new HitableList(list, i);
}


__device__ void simple_light_scene(Hitable **list, 
                                   Hitable **world){
    Texture *checker = new CheckerTexture(new ConstantTexture(vec3(0.8, 0.3, 0.1)),
                                          new ConstantTexture(vec3(0.9, 0.9, 0.9)));

    list[0] = new Sphere(vec3(0, -1000, 0), 1000, new Lambertian(checker));
    list[1] = new Sphere(vec3(0,     2, 0),    2, new Lambertian(checker));
    list[2] = new Sphere(vec3(0,     7, 0),    2, new DiffuseLight(new ConstantTexture(vec3(4, 4, 4))));
    list[3] = new RectangleXY(3, 5, 1, 3, -2,     new DiffuseLight(new ConstantTexture(vec3(4, 4, 4))));
    *world  = new HitableList(list, 4);
}


__device__ void cornell_box_scene(Hitable **list, 
                                  Hitable **world){
    int i = 0; 
    Material* red   = new   Lambertian(new ConstantTexture(vec3(0.65, 0.05, 0.05)));
    Material* white = new   Lambertian(new ConstantTexture(vec3(0.73, 0.73, 0.73)));
    Material* green = new   Lambertian(new ConstantTexture(vec3(0.12, 0.45, 0.15)));
    Material* light = new DiffuseLight(new ConstantTexture(vec3(  15,   15,   15)));

    Material* diele = new Dielectric(0.5);
    Material* metal = new Metal(vec3(0.7, 0.6, 0.5), 0.3);

    list[i++] = new FlipNormals(new RectangleYZ(  0, 555,   0, 555, 555, green));
    list[i++] =                (new RectangleYZ(  0, 555,   0, 555,   0,   red));
    list[i++] =                (new RectangleXZ(213, 343, 227, 332, 554, light));
    list[i++] = new FlipNormals(new RectangleXZ(  0, 555,   0, 555, 555, white));
    list[i++] =                (new RectangleXZ(  0, 555,   0, 555,   0, white));
    list[i++] = new FlipNormals(new RectangleXY(  0, 555,   0, 555, 555, white));

    list[i++] = new Translate(new Rotate(new Box(vec3(0, 0, 0), vec3(165, 165, 165), metal), -18), vec3(130, 0,  65));
    list[i++] = new Translate(new Rotate(new Box(vec3(0, 0, 0), vec3(165, 330, 165), diele),  18), vec3(265, 0, 295));
    
    *world = new HitableList(list, i);
}


__device__ void cornell_smoke_scene(Hitable **list, 
                                    Hitable **world){
    int i = 0; 
    Material* red   = new   Lambertian(new ConstantTexture(vec3(0.65, 0.05, 0.05)));
    Material* white = new   Lambertian(new ConstantTexture(vec3(0.73, 0.73, 0.73)));
    Material* green = new   Lambertian(new ConstantTexture(vec3(0.12, 0.45, 0.15)));
    Material* light = new DiffuseLight(new ConstantTexture(vec3(  15,   15,   15)));

    list[i++] = new FlipNormals(new RectangleYZ(  0, 555,   0, 555, 555, green));
    list[i++] =                (new RectangleYZ(  0, 555,   0, 555,   0,   red));
    list[i++] =                (new RectangleXZ(213, 343, 227, 332, 554, light));
    list[i++] = new FlipNormals(new RectangleXZ(  0, 555,   0, 555, 555, white));
    list[i++] =                (new RectangleXZ(  0, 555,   0, 555,   0, white));
    list[i++] = new FlipNormals(new RectangleXY(  0, 555,   0, 555, 555, white));

    Hitable* b1 = new Translate(new Rotate(new Box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130, 0,  65));
    Hitable* b2 = new Translate(new Rotate(new Box(vec3(0, 0, 0), vec3(165, 330, 165), white),  15), vec3(265, 0, 295));
    list[i++] = new ConstantMedium(b1, 0.01, new ConstantTexture(vec3(1.0, 1.0, 1.0)));
    list[i++] = new ConstantMedium(b2, 0.01, new ConstantTexture(vec3(0.0, 0.0, 0.0)));
    
    *world = new HitableList(list, i);
}


__device__ void final_scene(Hitable **list, 
                            Hitable **world, 
                            curandState *state){
    // NOTE: does not work now
    int nb = 20;
    Hitable** boxlist1 = new Hitable*[1000];
    Hitable** boxlist2 = new Hitable*[1000];
    Material* white  = new Lambertian(new ConstantTexture(vec3(0.73, 0.73, 0.73)));
    Material* ground = new Lambertian(new ConstantTexture(vec3(0.48, 0.83, 0.53)));
    
    int b = 0;
    for(int i = 0; i < nb; i++) {
        for(int j = 0; j < nb; j++) {
            float w = 100;
            float x0 = -1000 + i*w;
            float z0 = -1000 + j*w;
            float y0 = 0;
            float x1 = x0 + w;
            float y1 = 100 * (rand(state) + 0.01);
            float z1 = z0 + w;
            boxlist1[b++] = new Box(vec3(x0, y0, z0), vec3(x1, y1, z1), ground);
        }
    }

    int l = 0;
    list[l++] = new BVHNode(boxlist1, b, 0, 1, state);
    Material* light = new DiffuseLight(new ConstantTexture(vec3(7, 7, 7)));
    list[l++] = new RectangleXZ(123, 423, 147, 412, 554, light);
    vec3 center(400, 400, 200);
    list[l++] = new MovingSphere(center, center+vec3(30, 0, 0), 0, 1, 50, new Lambertian(new ConstantTexture(vec3(0.7, 0.3, 0.1))));
    list[l++] = new Sphere(vec3(260, 150,  45), 50, new Dielectric(1.5));
    list[l++] = new Sphere(vec3(  0, 150, 145), 50, new Metal(vec3(0.8, 0.8, 0.9), 10.0));
    Hitable* boundary = new Sphere(vec3(360, 150, 145), 70, new Dielectric(1.5));
    list[l++] = boundary;
    list[l++] = new ConstantMedium(boundary, 0.2, new ConstantTexture(vec3(0.2, 0.4, 0.9)));

    int ns = 1000;
    for(int j = 0; j < ns; j++){
        boxlist2[j] = new Sphere(vec3(165 * rand(state), 165 * rand(state), 165 * rand(state)), 10, white);   
    }
    list[l++] = new Translate(new Rotate(new BVHNode(boxlist2, ns, 0.0, 1.0, state), 15), vec3(-100, 270, 395));
    
    *world = new HitableList(list, l);
}
