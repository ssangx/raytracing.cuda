
#include "triangle.h"


Triangle::Triangle(vec3 vs[3],
                   vec3 n[3],
                   Material* mat,
                   const bool cull) : material(mat), backCulling(cull), EPSILON(0.00001) {
    for(int i = 0; i < 3; i++)  vertices[i] = vs[i];
    for(int i = 0; i < 3; i++)  normals[i] = n[i];
};


bool Triangle::hit(const Ray& r,
                   float t_min,
                   float t_max,
                   HitRecord& rec) const {

    // Möller –Trumbore intersection algorithm for triangle
    const vec3 &p0 = vertices[0];
    const vec3 &p1 = vertices[1];
    const vec3 &p2 = vertices[2];

    // Find vectors for two edges sharing p0
    vec3 e1 = p1 - p0;
    vec3 e2 = p2 - p0;
    // Begin calculating determinant - also used to calculate u parameter
    vec3 P = cross(r.direction(), e2);
    // if determinant is near zero, ray lies in plane of triangle
    float det = dot(e1, P);

    if (det < EPSILON && backCulling) {
        return false;
    }

    float inv_det = 1.f / det;

    //calculate distance from V1 to ray origin
    vec3 T = r.origin() - p0;

    //Calculate u parameter and test bound
    float u = dot(T, P) * inv_det;
    //The intersection lies outside of the triangle
    if(u < 0.f || u > 1.f) {
        return false;
    }

    //Prepare to test v parameter
    vec3 Q = cross(T, e1);

    //Calculate V parameter and test bound
    float v = dot(r.direction(), Q) * inv_det;
    //The intersection lies outside of the triangle
    if(v < 0.f || u + v  > 1.f) {
        return false;
    }

    float t = dot(e2, Q) * inv_det;

    const vec3& n0 = normals[0];
    const vec3& n1 = normals[1];
    const vec3& n2 = normals[2];

    // Interpolate to find normal
    vec3 n = (1 - u - v) * n0 + u * n1 + v * n2;

    rec.t = t;
    rec.p = r.point_at_t(rec.t);
    rec.normal = unit_vector(n);
    rec.mat_ptr = material;

    return true;
}


bool Triangle::bounding_box(AABB& bbox) const {
    float minX = std::min(vertices[0][0], std::min(vertices[1][0], vertices[2][0]));
    float minY = std::min(vertices[0][1], std::min(vertices[1][1], vertices[2][1]));
    float minZ = std::min(vertices[0][2], std::min(vertices[1][2], vertices[2][2]));

    float maxX = std::max(vertices[0][0], std::max(vertices[1][0], vertices[2][0]));
    float maxY = std::max(vertices[0][1], std::max(vertices[1][1], vertices[2][1]));
    float maxZ = std::max(vertices[0][2], std::max(vertices[1][2], vertices[2][2]));

    // Note that the triangle can be parallel to axis
    if(maxX - minX < 0.0001) { minX -= 0.0001; maxX += 0.0001; }
    if(maxY - minY < 0.0001) { minY -= 0.0001; maxY += 0.0001; }
    if(maxZ - minZ < 0.0001) { minZ -= 0.0001; maxZ += 0.0001; }

    bbox = AABB(vec3(minX, minY, minZ), vec3(maxX, maxY, maxZ));
    return true;
}
