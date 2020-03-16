# Ray Tracing


## Introduction

This is the CUDA implementation of ray-tracing algorithm. The project was initially inspired by [Ray Tracing In One Weekend/The Next Week](https://github.com/RayTracing/raytracing.github.io)


## How to run

```
cd build
cmake ../
make
./Raytracing
```


## Rendering results

![](https://github.com/HelloSangShen/raytracing.cuda/blob/master/images/image1.png)

![](https://github.com/HelloSangShen/raytracing.cuda/blob/master/images/image2.png)

![](https://github.com/HelloSangShen/raytracing.cuda/blob/master/images/image3.png)

A low resolution result for mesh rendering:
<p align="center">
  <img width="533" height="667" src="https://github.com/HelloSangShen/raytracing.cuda/blob/master/images/image4.png">
</p>


## Todo List

- [x] Finish the basic functionality
- [x] Load arbitrary model and rendering
    - [x] Rendering triangles
    - [x] Rendering mesh
- [x] Load scene from xml file
- [ ] Monte Carlo integration
