#ifndef MESH_H
#define MESH_H

#include <vector>

#include "bvh.h"
#include "hitable.h"
#include "triangle.h"
#include "objLoader.h"

using std::vector;


class Mesh: public Hitable {
public:
    Mesh(const std::string fn, Material* mat, float s);

    virtual bool hit(const Ray& r,
                     float t_min,
                     float t_max,
                     HitRecord& rec) const;

    virtual bool bounding_box(AABB& box) const;

    vector<vec3> points;
    vector<vec3> normals;
    vector<vec3> idxVertex;
    vector<vec3> idxNormal;

    Hitable* mesh;
    Material* material;

    int nPoints;
    int nTriangles;

    float scale;
};




#endif /* MESH_H */
