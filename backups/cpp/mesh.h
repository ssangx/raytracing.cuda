#ifndef MESH_H
#define MESH_H

#include <vector>

#include "hitable.h"
#include "triangle.h"
#include "obj_parser.h"
#include "hitable_list.h"

using std::vector;


class Mesh: public Hitable {
public:
    Mesh() {}
    Mesh(std::string fn, Material* mat);

    virtual bool hit(const Ray& r,
                     float t_min,
                     float t_max,
                     HitRecord& rec) const;

    virtual bool bounding_box(float t0,
                              float t1,
                              AABB& box) const;



    Hitable* mesh;

    vector<vec3> points;
    vector<vec3> normals;
    vector<vec3> idxVertex;
    vector<vec3> idxNormal;

    int nPoints;
    int nTriangles;

    Material* material;

    AABB bbox;

    void computeBoundingBox();
};


Mesh::Mesh(std::string fn, Material* mat) : material(mat) {

    parseObjByName(fn, points, normals, idxVertex, idxNormal);

    nPoints = points.size();
    nTriangles = idxVertex.size();

    // scaling
    for(vec3& p : points) p *= 20.0f;

    Hitable** triangles = new Hitable*[nTriangles];

    for (int i = 0; i < nTriangles; i++) {
        vec3 v[3] = {points[idxVertex[i][0]], points[idxVertex[i][1]], points[idxVertex[i][2]]};
        triangles[i] = new Triangle(v, material, true);
    }
    mesh = new HitableList(triangles, nTriangles);

    // compute the bbox
    computeBoundingBox();
}


bool Mesh::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const{
    return mesh->hit(r, t_min, t_max, rec);
}


bool Mesh::bounding_box(float t0, float t1, AABB& box) const {
   if(nPoints < 1) return false;
   box = bbox;
    return true;
}


#endif /* MESH_H */
