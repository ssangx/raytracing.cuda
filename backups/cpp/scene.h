#include "light.h"
#include "hitable.h"
#include <vector>


class Scene : public Hitable {
private:
    std::vector<Hitable*> objects;
    std::vector<Light*> lights;

public:
    Scene(const std::string& file_path);
    ~Scene();

    bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
    bool bounding_box(float t0, float t1, AABB& box) const;

    int numOfTriangles() const;
    int numOfObjects() const;
};


Scene::Scene(const std::string& file_path) {
    std::cout << "Loading from" << file_path << std::endl;

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
    double closest_so_far = t_max;
    for(const auto & object : objects){
        if(object->hit(r, t_min, closest_so_far, tmp_rec) && (tmp_rec.t < closest_so_far)){
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


int Scene::numOfTriangles() const {
    return objects.size();
}


int Scene::numOfObjects() const {
    // TODO
    return int(objects.size() * 3);
}
