#include <cmath>
#include <cstdio>
#include <iostream>

#include "scene.h"

#define RESOLUTION 1
#define SAMPLES 5000

#define checkCudaErrors(val) check_cuda((val), #val, __FILE__, __LINE__)


void check_cuda(cudaError_t result, 
                char const *const func, 
                const char *const file, 
                int const line) {
    if(result){
        std::cerr << "CUDA error = "<< static_cast<unsigned int>(result) << " at " <<
        file << ":" << line << " '" << func << "' \n";
        cudaDeviceReset();
        exit(99);
    }
}


__device__ vec3 shade(const Ray& r, 
                      Hitable **world, 
                      int depth,
                      curandState *state) {
    HitRecord rec;
    if((*world)->hit(r, 0.001, MAXFLOAT, rec)){
        Ray scattered;
        vec3 attenuation;
        vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
        if(depth < 15 && rec.mat_ptr->scatter(r, rec, attenuation, scattered, state)){
            return emitted + attenuation * shade(scattered, world, depth + 1, state);
        }else{
            return emitted;
        }
    }else{
        return vec3(0, 0, 0);
    }
}


__device__ vec3 shade_nolight(const Ray& r, 
                              Hitable **world, 
                              int depth,
                              curandState *state) {
    HitRecord rec;
    if((*world)->hit(r, 0.001, MAXFLOAT, rec)){
        Ray scattered;
        vec3 attenuation;
        if(depth < 15 && rec.mat_ptr->scatter(r, rec, attenuation, scattered, state)){
            return attenuation * shade_nolight(scattered, world, depth + 1, state);
        }
        else {
            return vec3(0, 0, 0);
        }
    }else{
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0-t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}


__global__ void build_scene(Hitable** obj_list, 
                            Hitable** world,
                            Camera** camera,
                            curandState *state,
                            int nx,
                            int ny,
                            int cnt){
    if(threadIdx.x == 0 && blockIdx.x == 0) {
        
        // random_scene(obj_list, world, state);
        // simple_light_scene(obj_list, world);
        // cornell_box_scene(obj_list, world);
        cornell_smoke_scene(obj_list, world, state);
        // bvh_scene(obj_list, world, state);
        // final_scene(obj_list, world, state);

        vec3 lookfrom(278, 278, -800);
        vec3 lookat(278, 278, 0);
        float dist_to_focus = 10.0;
        float aperture = 0.0;
        float vfov = 40.0;

        // vec3 lookfrom(13, 2, 3);
        // vec3 lookat(0, 0, 0);
        // float dist_to_focus = 10.0;
        // float aperture = 0.0;
        // float vfov = 20.0;

        *camera = new MotionCamera(lookfrom, 
                                lookat, 
                                vec3(0, 1, 0), 
                                vfov, 
                                float(nx) / float(ny), 
                                aperture, 
                                dist_to_focus,
                                0.0,
                                1.0);
    }
}


__global__ void random_init(int nx, 
                            int ny, 
                            curandState *state) {
    int x = threadIdx.x + blockIdx.x * blockDim.x;
    int y = threadIdx.y + blockIdx.y * blockDim.y;
    if((x >= nx) || (y >= ny)) return;
    int pixel_index = y * nx + x;
    curand_init(0, pixel_index, 0, &state[pixel_index]);
}


__global__ void destroy(Hitable** obj_list, 
                        Hitable** world,
                        Camera** camera, 
                        int obj_cnt) {
    for(int i = 0; i < obj_cnt; i++){
        delete *(obj_list + i);
    }
    delete *world;
    delete *camera;
}


__global__ void render(vec3* colorBuffer,
                       Hitable** world,
                       Camera** camera,
                       curandState* state,
                       int nx, 
                       int ny,
                       int samples) {
    int x = threadIdx.x + blockIdx.x * blockDim.x;
    int y = threadIdx.y + blockIdx.y * blockDim.y;
    if((x >= nx) || (y >= ny)) return;
    
    int pixel_index = y * nx + x;

    int ns = samples;
    vec3 col(0, 0, 0);
    for(int i = 0; i < ns; i++){
        float u = float(x + rand(&(state[pixel_index]))) / float(nx);
        float v = float(y + rand(&(state[pixel_index]))) / float(ny);
        Ray r = (*camera)->get_ray(u, v, state);
        col += shade(r, world, 0, &(state[pixel_index]));
        // col += shade_nolight(r, world, 0, &(state[pixel_index]));
    }
    col /= float(ns);
    col[0] = sqrt(col[0]);
    col[1] = sqrt(col[1]);
    col[2] = sqrt(col[2]);

    colorBuffer[pixel_index] = clip(col);
}


int main() {
    std::freopen("images/image.ppm", "w", stdout);

    int nx = 1024 * RESOLUTION;
    int ny = 512  * RESOLUTION;
    int tx = 16;
    int ty = 16;
    
    int num_pixel = nx * ny;

    // color buffer
    vec3 *colorBuffer;
    checkCudaErrors(cudaMallocManaged((void**)& colorBuffer, num_pixel*sizeof(vec3)));

    curandState* curand_state;
    checkCudaErrors(cudaMallocManaged((void**)& curand_state, num_pixel*sizeof(curandState)));

    // build world
    int obj_cnt = 488;
    Hitable** obj_list;
    Hitable** world;
    Camera**  camera;
    checkCudaErrors(cudaMallocManaged((void**)& obj_list, obj_cnt*sizeof(Hitable*)));
    checkCudaErrors(cudaMallocManaged((void**)& world, sizeof(Hitable*)));
    checkCudaErrors(cudaMallocManaged((void**)& camera, sizeof(Camera*)));

    dim3 blocks(nx / tx + 1, ny / ty + 1);
    dim3 threads(tx, ty);
    random_init <<<blocks, threads>>>(nx, ny, curand_state);
    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaDeviceSynchronize());

    build_scene <<<1, 1>>>(obj_list, world, camera, curand_state, nx, ny, obj_cnt);
    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaDeviceSynchronize());

    render <<<blocks, threads>>>(colorBuffer, world, camera, curand_state, nx, ny, SAMPLES);
    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaDeviceSynchronize());

    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    for(int i = ny - 1; i >= 0; i--){
        for(int j = 0; j < nx; j++){
            size_t pixel_index = i * nx + j;
            int ir = int(255.99 * colorBuffer[pixel_index].r());
            int ig = int(255.99 * colorBuffer[pixel_index].g());
            int ib = int(255.99 * colorBuffer[pixel_index].b());
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
    // clean up
    checkCudaErrors(cudaDeviceSynchronize());
    destroy<<<1, 1>>>(obj_list, world, camera, obj_cnt);

    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaFree(world));
    checkCudaErrors(cudaFree(obj_list));
    checkCudaErrors(cudaFree(camera));
    checkCudaErrors(cudaFree(curand_state));
    checkCudaErrors(cudaFree(colorBuffer));

    cudaDeviceReset();
}