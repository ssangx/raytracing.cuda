#include "scene.h"


Scene::Scene(const Context& _ctx): ctx(_ctx) {
    loadSceneFromXML(_ctx.xml_file, this);

    std::cout << objects.size() << " objects loaded" << std::endl;
}


Scene::~Scene() {
    for (auto obj : objects) delete obj;
    for (auto lit : lights) delete lit;
    objects.clear();
    lights.clear();
}


bool Scene::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
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


bool Scene::bounding_box(AABB& bbox) const {
    if(objects.size() < 1) return false;

    AABB tmp_box;
    bool first_true = objects[0]->bounding_box(tmp_box);

    if(!first_true) {
        return false;
    }else {
        bbox = tmp_box;
    }

    for(int i = 1; i < objects.size(); i++){
        if(objects[0]->bounding_box(tmp_box)){
            bbox = surrounding_box(bbox, tmp_box);
        }else{
            return false;
        }
    }
    return true;
}
