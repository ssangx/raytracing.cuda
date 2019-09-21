#define STB_IMAGE_IMPLEMENTATION
#ifndef SCENEH
#define SCENEH

#include "ray.h"
#include "box.h"
#include "camera.h"
#include "sphere.h"
#include "material.h"
#include "texture.h"
#include "hitable_list.h"
#include "stb_image.h"
#include "rectangle.h"
#include "transform.h"
#include "constant_medium.h"
#include "bvh_node.h"


vec3 color(const Ray& r, Hitable* world, int depth){
    HitRecord rec;
    if(world->hit(r, 0.001, MAXFLOAT, rec)){
        Ray scattered;
        vec3 attenuation;
        vec3 emmited = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)){
            return emmited + attenuation * color(scattered, world, depth + 1);
        }else{
            return emmited;
        }
    }
    return vec3(0, 0, 0);
}


Hitable* random_scene(){
    int n = 500; 
    Hitable **list = new Hitable*[n+1];
    Texture* checker = new CheckerTexture(new ConstantTexture(vec3(0.2, 0.3, 0.1)), 
                                          new ConstantTexture(vec3(0.9, 0.9, 0.9)));
    list[0] = new Sphere(vec3(0, -1000, 0), 1000, new Lambertian(checker));
    int i = 1;
    for(int a = -10; a < 10; a ++){
        for(int b = -10; b < 10; b ++){
            float choose_mat = drand48();
            vec3 center(a + 0.9 * drand48(), 0.2, b + 0.9 * drand48());
            if((center - vec3(4, 0.2, 0)).length() > 0.9){
                if (choose_mat < 0.8){
                    Texture* rand_texture = new ConstantTexture(vec3(drand48()*drand48(), 
                                                                     drand48()*drand48(), 
                                                                     drand48()*drand48()));
                    list[i++] = new MovingSphere(center, center + vec3(0, 0.5 * drand48(), 0), 0.0, 1.0, 0.2,  
                                    new Lambertian(rand_texture));
                }else if(choose_mat < 0.95){
                    list[i++] = new Sphere(center, 0.2,
                                    new Metal(vec3(0.5*(1+drand48()), 0.5*(1+drand48()), 0.5*(1+drand48())), 0.5*drand48()));
                }else{
                    list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
                }
            }
        }
    }
    
    Texture* tmp_texture = new ConstantTexture(vec3(0.4, 0.2, 0.1));
    list[i++] = new Sphere(vec3(0,  1, 0),  1.0, new Dielectric(1.5));
    list[i++] = new Sphere(vec3(-4, 1, 0), 1.0, new Lambertian(tmp_texture));
    list[i++] = new Sphere(vec3(4,  1, 0),  1.0, new Metal(vec3(0.7, 0.6, 0.5), 0.0));
    return new HitableList(list, i);
}


Hitable* two_spheres(){
    Hitable **list = new Hitable*[2];
    Texture* checker = new CheckerTexture(new ConstantTexture(vec3(0.2, 0.3, 0.1)), 
                                          new ConstantTexture(vec3(0.9, 0.9, 0.9)));
    list[0] = new Sphere(vec3(0, -10, 0), 10, new Lambertian(checker));
    list[1] = new Sphere(vec3(0, 10,  0), 10, new Lambertian(checker));
    return new HitableList(list, 2);
}


Hitable* two_perlin_sphere(){
    Texture* perlin_text = new NoiseTexture(1);
    Hitable **list = new Hitable*[2];
    list[0] = new Sphere(vec3(0, -1000, 0), 1000, new Lambertian(perlin_text));
    list[1] = new Sphere(vec3(0,     2, 0),    2, new Lambertian(perlin_text));
    return new HitableList(list, 2);
}


Hitable* image_texture(){
    int nx, ny, nn;
    unsigned char* tex_data = stbi_load("test.jpg", &nx, &ny, &nn, 0);
    Material* mat = new Lambertian(new ImageTexture(tex_data, nx, ny));
    Hitable **list = new Hitable*[1];
    list[0] = new Sphere(vec3(0, 2, 0), 2, mat);
    return new HitableList(list, 1);
}


Hitable* sample_light(){
    Texture* pertext = new NoiseTexture(4);
    Hitable** list = new Hitable*[4];

    list[0] = new Sphere(vec3(0, -1000, 0), 1000, new Lambertian(pertext));
    list[1] = new Sphere(vec3(0,     2, 0),    2, new Lambertian(pertext));
    list[2] = new Sphere(vec3(0,     7, 0),    2, new DiffuseLight(new ConstantTexture(vec3(4, 4, 4))));
    list[3] = new RectangleXY(3, 5, 1, 3, -2, new DiffuseLight(new ConstantTexture(vec3(4, 4, 4))));
    return new HitableList(list, 4);
}


Hitable* cornell_box(){
    Hitable** list = new Hitable*[10];
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
    list[i++] = new Translate(new Rotate(new Box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130, 0,  65));
    list[i++] = new Translate(new Rotate(new Box(vec3(0, 0, 0), vec3(165, 330, 165), white),  18), vec3(265, 0, 295));

    return new HitableList(list, i);
}


Hitable* cornell_smoke(){
    Hitable** list = new Hitable*[8];
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
    
    return new HitableList(list, i);
}



Hitable* final(){
    int nb = 20;
    Hitable** list = new Hitable*[30];
    Hitable** boxlist1 = new Hitable*[10000];
    Hitable** boxlist2 = new Hitable*[10000];
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
            float y1 = 100 * (drand48() + 0.01);
            float z1 = z0 + w;
            boxlist1[b++] = new Box(vec3(x0, y0, z0), vec3(x1, y1, z1), ground);
        }
    }

    int l = 0;
    list[l++] = new BVHNode(boxlist1, b, 0, 1);
    Material* light = new DiffuseLight(new ConstantTexture(vec3(7, 7, 7)));
    list[l++] = new RectangleXZ(123, 423, 147, 412, 554, light);
    vec3 center(400, 400, 200);
    list[l++] = new MovingSphere(center, center+vec3(30, 0, 0), 0, 1, 50, new Lambertian(new ConstantTexture(vec3(0.7, 0.3, 0.1))));
    list[l++] = new Sphere(vec3(260, 150,  45), 50, new Dielectric(1.5));
    list[l++] = new Sphere(vec3(  0, 150, 145), 50, new Metal(vec3(0.8, 0.8, 0.9), 10.0));
    Hitable* boundary = new Sphere(vec3(360, 150, 145), 70, new Dielectric(1.5));
    list[l++] = boundary;
    list[l++] = new ConstantMedium(boundary, 0.2, new ConstantTexture(vec3(0.2, 0.4, 0.9)));

    // int nx, ny, nn;
    // unsigned char* tex_data = stbi_load("/source/earthmap.jpg", &nx, &ny, &nn, 0);
    // Material* emat = new Lambertian(new ImageTexture(tex_data, nx, ny));
    // list[l++] = new Sphere(vec3(400, 200, 400), 100, emat);
    Texture* pertext = new NoiseTexture(0.1);
    list[l++] = new Sphere(vec3(220, 280, 300), 80, new Lambertian(pertext));
    
    int ns = 1000;
    for(int j = 0; j < ns; j++){
        boxlist2[j] = new Sphere(vec3(165 * drand48(), 165 * drand48(), 165 * drand48()), 10, white);   
    }
    list[l++] = new Translate(new Rotate(new BVHNode(boxlist2, ns, 0.0, 1.0), 15), vec3(-100, 270, 395));
    
    return new HitableList(list, l);
}

#endif