#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "box.h"
#include "mesh.h"
#include "light.h"
#include "hitable.h"
#include "objloader.h"
#include "material.h"
#include "sphere.h"
#include "rectangle.h"
#include "transform.h"


class Scene : public Hitable {
private:
    std::vector<Hitable*> objects;
    std::vector<Light*> lights;

public:
    Scene(const std::string& file_path);
    ~Scene();

    bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
    bool bounding_box(float t0, float t1, AABB& bbox) const;
};


Scene::Scene(const std::string& file_path) {
    // TODO: load scene from xml files
    std::cout << "Loading from " << file_path << std::endl;

    // objects.push_back(new Mesh(file_path, new Normal()));

    Material* red   = new   Lambertian(new ConstantTexture(vec3(0.65, 0.05, 0.05)));
    Material* white = new   Lambertian(new ConstantTexture(vec3(0.73, 0.73, 0.73)));
    Material* green = new   Lambertian(new ConstantTexture(vec3(0.12, 0.45, 0.15)));
    Material* light = new DiffuseLight(new ConstantTexture(vec3(  15,   15,   15)));

    objects.push_back(new FlipNormals(new RectangleYZ(  0, 555,   0, 555, 555, green)));
    objects.push_back(               (new RectangleYZ(  0, 555,   0, 555,   0,   red)));
    objects.push_back(               (new RectangleXZ(200, 360, 215, 360, 554, light)));
    objects.push_back(new FlipNormals(new RectangleXZ(  0, 555,   0, 555, 555, white)));
    objects.push_back(               (new RectangleXZ(  0, 555,   0, 555,   0, white)));
    objects.push_back(new FlipNormals(new RectangleXY(  0, 555,   0, 555, 555, white)));

    Material* diele = new Dielectric(0.5);
    Material* metal = new Metal(vec3(0.7, 0.6, 0.5), 0.3);

    objects.push_back(new Translate(new Rotate(new Mesh(file_path, metal), 180.f), vec3(348, 128, 400)));
    objects.push_back(new Translate(new Rotate(new Box(vec3(0, 0, 0), vec3(165, 165, 165), diele), -18), vec3(130, 0,  65)));

    std::cout << objects.size() << " objects loaded" << std::endl;
}


Scene::~Scene() {
    for (auto obj : objects) delete obj;
    for (auto lit : lights) delete lit;
    objects.clear();
    lights.clear();
}


bool Scene::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const{
    HitRecord tmp_rec;
    bool hit_anything = false;
    float closest_so_far = t_max;

    for(const auto & object : objects) {
        if(object->hit(r, t_min, closest_so_far, tmp_rec) & (tmp_rec.t < closest_so_far)) {
            hit_anything = true;
            closest_so_far = tmp_rec.t;
            rec = tmp_rec;
        }
    }

    return hit_anything;
}


bool Scene::bounding_box(float t0, float t1, AABB& bbox) const {
    if(objects.size() < 1) return false;

    AABB tmp_box;
    bool first_true = objects[0]->bounding_box(t0, t1, tmp_box);

    if(!first_true) {
        return false;
    }else {
        bbox = tmp_box;
    }

    for(int i = 1; i < objects.size(); i++){
        if(objects[0]->bounding_box(t0, t1, tmp_box)){
            bbox = surrounding_box(bbox, tmp_box);
        }else{
            return false;
        }
    }
    return true;
}


#endif