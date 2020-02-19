cmake_minimum_required(VERSION 3.10)
project(RayTracing)

find_package(CUDA REQUIRED)

# set gcc version
set(CMAKE_CXX_STANDARD 11)

# set nvcc compiler
set(CUDA_NVCC_FLAGS -gencode arch=compute_60,code=sm_60;-G;-g)

include_directories(src)
include_directories(images)

link_directories(/usr/lib)
link_directories(/usr/local/lib)

cuda_add_executable(RayTracing src/main.cu)