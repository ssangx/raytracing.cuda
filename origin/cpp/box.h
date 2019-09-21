#ifndef BOXH
#define BOXH

#include "material.h"
#include "rectangle.h"
#include "hitable_list.h"


class Box: public Hitable{
public:
    Box() {}
    Box(const vec3& p0, const vec3& p1, Material* ptr);

    virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
    virtual bool bounding_box(float t0, float t1, AABB& box) const{
        box = AABB(p_min, p_max);
        return true;
    }

    vec3 p_min, p_max;
    Hitable* list_ptr;
};

Box::Box(const vec3& p0, const vec3& p1, Material* ptr){
    p_min = p0;
    p_max = p1;

    Hitable** list = new Hitable*[6];
    list[0] =                (new RectangleXY(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr));
    list[1] = new FlipNormals(new RectangleXY(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));
    list[2] =                (new RectangleXZ(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr));
    list[3] = new FlipNormals(new RectangleXZ(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));
    list[4] =                (new RectangleYZ(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr));
    list[5] = new FlipNormals(new RectangleYZ(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
    list_ptr = new HitableList(list, 6);
}

bool Box::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const{
    return list_ptr->hit(r, t_min, t_max, rec);
}


#endif