#ifndef XMLLOADER_H
#define XMLLOADER_H

#include <string.h>

#include "vec3.h"
#include "scene.h"
#include "camera.h"
#include "material.h"
#include "pugixml.h"


class Scene;  // forward declaration


class SceneTraverser: public pugi::xml_tree_walker {
public:
    Scene* scene;
    virtual bool for_each(pugi::xml_node& node);
};


void loadSceneFromXML(const std::string& file_path, Scene* scene);

#endif