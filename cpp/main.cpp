#include <ctime>
#include <iostream>
#include <unistd.h>
#include "scene.h"
#include "camera.h"
#include "rectangle.h"


vec3 shade(const Ray& r, Hitable* scene, int depth){
    HitRecord rec;
    if(scene->hit(r, 0.001, FLT_MAX, rec)) {
        Ray scattered;
        vec3 attenuation;
        vec3 emmited = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
        if (depth < 15 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return emmited + attenuation * shade(scattered, scene, depth + 1);
        } else {
            return emmited;
        }
    }
    return vec3(0, 0, 0);
}


vec3 shade_nolight(const Ray& r, Hitable* scene, int depth) {
    HitRecord rec;
    if (scene->hit(r, 0.001, FLT_MAX, rec)) {
        Ray scattered;
        vec3 attenuation;
        if (depth < 15 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * shade_nolight(scattered, scene, depth + 1);
        } else {
            return vec3(0, 0, 0);
        }
    } else {
        return vec3(1.0, 1.0, 1.0);
    }
}


int main() {

    std::time_t tic = std::time(NULL);
    std::cout << "Start running at " << std::asctime(std::localtime(&tic)) << std::endl;

    std::string prefix = "/home/ssang/Main/Experiment/Raytracing/cpp";
    std::ofstream imgWrite(prefix + "/images/image.ppm");

    int nx = 800;
    int ny = 1000;
    int ns = 200;
    imgWrite << "P3\n" << nx << " " << ny <<"\n255\n";

    Context ctx;
    ctx.xml_file = prefix + "/data/scenes/cornell_bunny.xml";
    ctx.root = prefix;
    ctx.sample = ns;
    ctx.height = ny;
    ctx.width  = nx;

    Scene* scene = new Scene(ctx);

    const float gamma = 1 / 2.2;
    for(int j = 0; j < ny; j ++) {
        for(int i = 0; i < nx; i ++) {
            vec3 col(0, 0, 0);
            for(int s = 0; s < ns; s++) {
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                Ray r = scene->camera->castRay(u, v);
                col += shade(r, scene, 0);
            }
            col /= float(ns);

            // gamma correction
            col = vec3(pow(col[0], gamma), pow(col[1], gamma), pow(col[2], gamma));

            int ir = int(255.9 * col[0]);
            int ig = int(255.9 * col[1]);
            int ib = int(255.9 * col[2]);

            imgWrite << ir << " " << ig << " " << ib << "\n";
        }
    }

    std::time_t toc = std::time(NULL);
    std::cout << "Finish running at " << std::asctime(std::localtime(&toc)) << std::endl;
    std::cout << "Time consuming " << toc - tic << std::endl;

    return 0;
}