#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <unordered_map>

#include "box.h"
#include "mesh.h"
#include "light.h"
#include "hitable.h"
#include "objLoader.h"
#include "material.h"
#include "sphere.h"
#include "rectangle.h"
#include "transform.h"
#include "context.h"
#include "xmlLoader.h"
#include "camera.h"


class Scene : public Hitable {
public:
    Scene(const Context& _ctx);
    ~Scene();

    bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
    bool bounding_box(AABB& bbox) const;

    std::vector<Hitable*> objects;
    std::vector<Light*> lights;

    std::unordered_map<std::string, Material*> materials;
    Context ctx;

    Camera* camera;
};


#endif