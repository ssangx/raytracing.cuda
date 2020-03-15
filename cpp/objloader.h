#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include "vec3.h"


vec3 computeMean(const std::vector<vec3>& points);
void centering(std::vector<vec3>& points, vec3& mean);
void scaling(std::vector<vec3>& points, vec3& mean);

std::pair<int, int> parseFaceIndex (std::string str);

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

vec3 computeMean(const std::vector<vec3>& points) {
    vec3 mean(0, 0, 0);
    for(vec3 pt: points)
        mean += pt;
    return mean / float(points.size());
}


void centering(std::vector<vec3>& points, vec3& mean) {
    for(int i = 0; i < points.size(); i++)
        points[i] -= mean;
}


void scaling(std::vector<vec3>& points, vec3& mean) {
    float max_dis = 0;

    for(vec3 pt: points){
        float dis = (pt - mean).length();
        if(dis > max_dis) max_dis = dis;
    }

    for(int i = 0; i < points.size(); i++)
        points[i] /= max_dis;
}


#endif /* OBJ_PARSER_H */
