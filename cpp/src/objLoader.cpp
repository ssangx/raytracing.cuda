#include "objLoader.h"


void parseObjByName(const std::string fn,
                    std::vector<vec3>& points,
                    std::vector<vec3>& normals,
                    std::vector<vec3>& idxVertex,
                    std::vector<vec3>& idxNormal) {
    std::ifstream objFile(fn); // The obj file we are reading.

    // Check whether the file can be opened.
    if (objFile.is_open()) {
        std::string line; // A line in the file.

        // Read lines from the file.
        while (std::getline(objFile, line)){
            // Turn the line into a string stream for processing.
            std::stringstream ss;
            ss << line;

            // Read the first word of the line.
            std::string label;
            ss >> label;

            // If the line is about vertex (starting with a "v").
            if (label == "v"){
                // Read the later three float numbers and use them as the
                // coordinates.
                vec3 point;
                ss >> point[0] >> point[1] >> point[2];
                // Process the point. For example, you can save it to a.
                points.push_back(point);
            } else if (label == "vn"){
                // read normals
                vec3 normal;
                ss >> normal[0] >> normal[1] >> normal[2];
                normals.push_back(normal);
            } else if (label == "f"){
                // read face, order: vertex / uv / normal
                std::string p1, p2, p3;
                ss >> p1 >> p2 >> p3;

                std::pair<int, int> face[3];
                face[0] = parseFaceIndex(p1);
                face[1] = parseFaceIndex(p2);
                face[2] = parseFaceIndex(p3);
                idxVertex.push_back(vec3(face[0].first, face[1].first, face[2].first));
                idxNormal.push_back(vec3(face[0].second, face[1].second, face[2].second));
            }
        }
    } else {
        std::cerr << "Can't open the file " << fn << std::endl;
    }
}


std::pair<int, int> parseFaceIndex (std::string str) {
    std::stringstream ss(str);
    std::vector<std::string> ret;

    std::string tmp;
    while(getline(ss, tmp, '/')) {
        ret.push_back(tmp);
    }
    return std::make_pair<int, int>(int(std::stoi(ret[0])), std::stoi(ret[2]));
}