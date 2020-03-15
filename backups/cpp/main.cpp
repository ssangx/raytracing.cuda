#include <ctime>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include "scene.h"
#include "test_scene.h"



int main() {

    std::time_t tic = std::time(NULL);
    std::cout << "Start running at " << std::asctime(std::localtime(&tic)) << std::endl;

    std::ofstream imgWrite("images/image.ppm");

    int nx = 400;
    int ny = 200;
    int ns = 100;
    imgWrite << "P3\n" << nx << " " << ny <<"\n255\n";

    Hitable* world = final();
    
    vec3 lookfrom(278, 278, -800);
    vec3 lookat(278,278,0);

    float dist_to_focus = 10.0;
    float aperture = 0.0;
    float vfov = 40.0;
    Camera cam(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx)/float(ny),
                    aperture, dist_to_focus, 0.0, 1.0);

    for(int j = ny - 1; j >= 0; j --){
        for(int i = 0; i < nx; i ++){
            vec3 col(0, 0, 0);
            for(int s = 0; s < ns; s++){
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                Ray r = cam.get_ray(u, v);
                col += color(r, world, 0);
            }
            col /= float(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
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