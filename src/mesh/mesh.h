#ifndef MESH_H
#define MESH_H

#include "hitable/hitable.h"
#include "shapes/triangle.h"
#include "hitable/hitable_list.h"


// __device__ float min(const float a, const float b) {
//     if(a > b) return b;
//     return a;
// }


class Mesh: public Hitable {
public:
    __device__ Mesh() {}
    __device__ Mesh(Hitable* m, Material* mat, vec3* p, vec3* n, int np, int nt);

    __device__ virtual bool hit(const Ray& r,
                                float t_min,
                                float t_max,
                                HitRecord& rec) const;

    __device__ virtual bool bounding_box(float t0,
                                         float t1,
                                         AABB& box) const;
        
    __device__ void computeBoundingBox();

    Hitable* mesh;

    vec3* points;
    vec3* normals;

    int nPoints;
    int nTriangles;

    Material* material;

    AABB bbox;

};


__device__ Mesh::Mesh(Hitable* m, Material* mat, vec3* p, vec3* n, 
                      int np, int nt) : mesh(m), material(mat) {

    points  = p;
    normals = n;
    
    nPoints = np;
    nTriangles = nt;

    // compute the bbox
    computeBoundingBox();
}


__device__ bool Mesh::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
    return mesh->hit(r, t_min, t_max, rec);
}


__device__ bool Mesh::bounding_box(float t0, float t1, AABB& box) const {
    if(nPoints < 1) return false;
    box = bbox;
    return true;
}


__device__ void Mesh::computeBoundingBox() {
    // compute bounding box
    float minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX;
    float maxX = FLT_MIN, maxY = FLT_MIN, maxZ = FLT_MIN;

    vec3 p;
    for (int i = 0; i < nPoints; i++) {
        p = *(points + i);
        minX = min(p[0], minX);
        minY = min(p[1], minY);
        minZ = min(p[2], minZ);

        maxX = max(p[0], maxX);
        maxY = max(p[1], maxY);
        maxZ = max(p[2], maxZ);
    }

    bbox = AABB(vec3(minX, minY, minZ), vec3(maxX, maxY, maxZ));
}

#endif /* MESH_H */
