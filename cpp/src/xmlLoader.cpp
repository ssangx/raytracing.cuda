#include "xmlLoader.h"


bool SceneTraverser::for_each(pugi::xml_node& node) {
    if(strncmp(node.name(),"Material", 8) == 0) {
        std::string id = node.attribute("id").value();
        std::string type = node.attribute("type").value();

        vec3 color;
        color[0] = std::stof(node.child("color").attribute("r").value());
        color[1] = std::stof(node.child("color").attribute("g").value());
        color[2] = std::stof(node.child("color").attribute("b").value());

        Material* m;
        if(type == "Lambertian") {
            m = new Lambertian(new ConstantTexture(color));

        } else if(type == "DiffuseLight") {
            float intensity = std::stof(node.attribute("intensity").value());
            m = new DiffuseLight(new ConstantTexture(color * intensity));

        } else if(type == "Dielectric") {
            float ri = std::stof(node.attribute("ri").value());
            m = new Dielectric(ri);

        } else if(type == "Metal") {
            float fuzz = std::stof(node.attribute("fuzz").value());
            m = new Metal(color, fuzz);

        } else {
            std::cout <<  "Material " << type << "is not supported now" << std::endl;
        }

        scene->materials[id] = m;

    } else if(strncmp(node.name(), "Primitive", 9) == 0) {
        std::string material_id = node.attribute("material_id").value();
        std::string type = node.attribute("type").value();
        std::string flip = node.attribute("flip").value();
        Hitable* object;

        if (type == "RectangleXY" || type == "RectangleYZ" || type == "RectangleXZ") {
            float a = std::stof(node.child("params").attribute("a").value());
            float b = std::stof(node.child("params").attribute("b").value());
            float c = std::stof(node.child("params").attribute("c").value());
            float d = std::stof(node.child("params").attribute("d").value());
            float e = std::stof(node.child("params").attribute("e").value());

            if(type == "RectangleXY") object = new RectangleXY(a, b, c, d, e, scene->materials[material_id]);
            else if (type == "RectangleYZ") object = new RectangleYZ(a, b, c, d, e, scene->materials[material_id]);
            else if (type == "RectangleXZ") object = new RectangleXZ(a, b, c, d, e, scene->materials[material_id]);

            if(flip == "true") object = new FlipNormals(object);
        }

        scene->objects.push_back(object);

    } else if(strncmp(node.name(),"Geometry", 8) == 0) {
        std::string material_id = node.attribute("material_id").value();
        std::string file_path = node.attribute("file_path").value();
        Hitable* object;

        float scale = std::stof(node.attribute("scale").value());

        vec3 translate;
        translate[0] = std::stof(node.child("translate").attribute("x").value());
        translate[1] = std::stof(node.child("translate").attribute("y").value());
        translate[2] = std::stof(node.child("translate").attribute("z").value());

        float rotate = std::stof(node.child("rotate").attribute("y").value());

        object = new Translate(new Rotate(new Mesh(scene->ctx.root + file_path, scene->materials[material_id], scale), rotate), translate);
        scene->objects.push_back(object);

    } else if(strncmp(node.name(), "Camera", 6) == 0) {
        float dist_to_focus = std::stof(node.attribute("dist_to_focus").value());
        float aperture = std::stof(node.attribute("aperture").value());
        float vfov = std::stof(node.attribute("vfov").value());

        vec3 lookFrom;
        lookFrom[0] = std::stof(node.child("lookFrom").attribute("x").value());
        lookFrom[1] = std::stof(node.child("lookFrom").attribute("y").value());
        lookFrom[2] = std::stof(node.child("lookFrom").attribute("z").value());

        vec3 lookAt;
        lookAt[0] = std::stof(node.child("lookAt").attribute("x").value());
        lookAt[1] = std::stof(node.child("lookAt").attribute("y").value());
        lookAt[2] = std::stof(node.child("lookAt").attribute("z").value());

        vec3 up;
        up[0] = std::stof(node.child("up").attribute("x").value());
        up[1] = std::stof(node.child("up").attribute("y").value());
        up[2] = std::stof(node.child("up").attribute("z").value());

        scene->camera = new Camera(lookFrom, lookAt, up, vfov,
                float(scene->ctx.width)/float(scene->ctx.height), aperture, dist_to_focus);

    }

    return true;
}


void loadSceneFromXML(const std::string& file_path, Scene* scene) {
    pugi::xml_document xmlDoc;
    std::cout << "Loading XML file: " << file_path << std::endl;
    pugi::xml_parse_result result = xmlDoc.load_file(file_path.c_str());

    SceneTraverser walker;
    walker.scene = scene;

    xmlDoc.traverse(walker);
    std::cout << "Scene created!" << std::endl;
}