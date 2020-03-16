#include "mesh.h"


Mesh::Mesh(const std::string fn, Material* mat, float s=1.0f) : material(mat) {
    parseObjByName(fn, points, normals, idxVertex, idxNormal);

    // scaling
    scale = s;

    for(vec3& p : points) p *= scale;

    nPoints = points.size();
    nTriangles = idxVertex.size();

    Hitable** triangles = new Hitable*[nTriangles];

    for (int i = 0; i < nTriangles; i++) {
        vec3 v[3] = { points[idxVertex[i][0]-1], points[idxVertex[i][1]-1], points[idxVertex[i][2]-1] };
        vec3 n[3] = { normals[idxNormal[i][0]-1], normals[idxNormal[i][1]-1], normals[idxNormal[i][2]-1] };
        triangles[i] = new Triangle(v, n, material, true);
    }

    // Save the mesh as a BVH directly
    mesh = new BVHNode(triangles, nTriangles);
}


bool Mesh::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const{
    bool ret = mesh->hit(r, t_min, t_max, rec);
    return ret;
}


bool Mesh::bounding_box(AABB& bbox) const {
    bool ret = mesh->bounding_box(bbox);
    return  ret;
}