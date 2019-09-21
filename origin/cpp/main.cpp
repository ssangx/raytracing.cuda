#include <cstdio>
#include <iostream>
#include "scene.h"


int main(){
    std::freopen("image.ppm", "w", stdout);

    int nx = 2000;
    int ny = 1000;
    int ns = 1000;
    std::cout << "P3\n" << nx << " " << ny <<"\n255\n";
    
    // Hitable* world = random_scene();
    // Hitable* world = two_spheres();
    // Hitable* world = two_perlin_sphere();
    // Hitable* world = image_texture();
    // Hitable* world = sample_light();
    // Hitable* world = cornell_box();
    // Hitable* world = cornell_smoke();
    Hitable* world = final();

    vec3 lookfrom(278, 278, -800);
    vec3 lookat(278, 278, 0);
    float dist_to_focus = 10.0;
    float aperture = 0.0;
    float vfov = 40.0;
    Camera cam(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx)/float(ny), aperture, dist_to_focus, 0.0, 1.0);

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

            std::cout << ir << " " << ig << " " << ib << "  # " << i + (ny - j - 1) * nx << "\n";
        }
    }
}