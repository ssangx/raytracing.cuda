#ifndef MESH_H
#define MESH_H

#include <vector>

#include "bvh.h"
#include "hitable.h"
#include "triangle.h"
#include "objloader.h"
#include "hitable_list.h"

using std::vector;


class Mesh: public Hitable {
public:
    Mesh(const std::string fn, Material* mat);

    virtual bool hit(const Ray& r,
                     float t_min,
                     float t_max,
                     HitRecord& rec) const;

    virtual bool bounding_box(float t0,
                              float t1,
                              AABB& box) const;

    vector<vec3> points;
    vector<vec3> normals;
    vector<vec3> idxVertex;
    vector<vec3> idxNormal;

    Hitable* mesh;
    Material* material;

    int nPoints;
    int nTriangles;
};


Mesh::Mesh(const std::string fn, Material* mat) : material(mat) {
    parseObjByName(fn, points, normals, idxVertex, idxNormal);

    // scaling
    for(vec3& p : points) p *= 80.0f;

    nPoints = points.size();
    nTriangles = idxVertex.size();

    Hitable** triangles = new Hitable*[nTriangles];

    for (int i = 0; i < nTriangles; i++) {
        vec3 v[3] = { points[idxVertex[i][0]-1], points[idxVertex[i][1]-1], points[idxVertex[i][2]-1] };
        vec3 n[3] = { normals[idxNormal[i][0]-1], normals[idxNormal[i][1]-1], normals[idxNormal[i][2]-1] };
        triangles[i] = new Triangle(v, n, material, true);
    }

    // Save the mesh as a BVH directly
    mesh = new BVHNode(triangles, nTriangles, 0.0, 1.0);
}


bool Mesh::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const{
    bool ret = mesh->hit(r, t_min, t_max, rec);
    return ret;
}


bool Mesh::bounding_box(float t0, float t1, AABB& bbox) const {
    bool ret = mesh->bounding_box(t0, t1, bbox);
    return  ret;
}


#endif /* MESH_H */
