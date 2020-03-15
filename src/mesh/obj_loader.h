#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include <stdlib.h>
#include "core/vec3.h"


vec3 computeMean(vec3* points, int np);
void centering(vec3* points, vec3& mean, int np);
void scaling(vec3* points, vec3& mean, int np);

std::pair<int, int> parseFaceIndex (std::string str);

void parseObjByName(std::string fn,
                    vec3* points,
                    vec3* idxVertex,
                    int& nPoints,
                    int& nTriangles) {

    std::ifstream objFile(fn.c_str()); // The obj file we are reading.
    
    int np = 0, nn = 0, nt = 0;
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
                points[np++] = point;
            } else if (label == "vn"){
                // read normals
                // vec3 normal;
                // ss >> normal[0] >> normal[1] >> normal[2];
                // normals[nn++] = normal;
            } else if (label == "f"){
                // read small bunny

                vec3 idx;
                ss >> idx[0] >> idx[1] >> idx[2];
                idxVertex[nt++] = idx;
                // read normals
                // std::string p1, p2, p3;
                // ss >> p1 >> p2 >> p3;

                // std::pair<int, int> face[3];
                // face[0] = parseFaceIndex(p1);
                // face[1] = parseFaceIndex(p2);
                // face[2] = parseFaceIndex(p3);

                // idxVertex[nt] = vec3(face[0].first,  face[1].first,  face[2].first);
                // // idxNormal[nt] = vec3(face[0].second, face[1].second, face[2].second);
                // nt ++;

                // vec3 idx;
                // ss >> idx[0] >> idx[1] >> idx[2];
                // idxVertex[nt++] = idx;
            }
        }
    } else {
        std::cerr << "Can't open the file " << fn << std::endl;
    }

    nPoints = np;
    nTriangles = nt;
    vec3 mean = computeMean(points, np);
    centering(points, mean, np);
    // scaling(points, mean, np);

    // float x = FLT_MIN, y = FLT_MIN, z = FLT_MIN;
    // for(int i = 0; i < np; i++) {
    //     x = std::max(x, points[i].x());
    //     y = std::max(y, points[i].y());
    //     z = std::max(z, points[i].z());
    // }

    // std::cout << "MAX: " << vec3(x, y, z) << std::endl;

    // x = FLT_MAX, y = FLT_MAX, z = FLT_MAX;
    // for(int i = 0; i < np; i++) {
    //     x = std::min(x, points[i].x());
    //     y = std::min(y, points[i].y());
    //     z = std::min(z, points[i].z());
    // }

    // std::cout << "MIN: " << vec3(x, y, z) << std::endl;
}

std::pair<int, int> parseFaceIndex(std::string str) {
    std::size_t pos = str.find("//");
    std::string w1 = str.substr(0, pos);   // v
    std::string w2 = str.substr(pos + 2);  // vn
    return std::make_pair(std::atoi(w1.c_str()) - 1, std::atoi(w2.c_str()) - 1);
}

vec3 computeMean(vec3* points, int np) {
    vec3 mean(0, 0, 0);
    for(int i = 0; i < np; i++) {
        mean += points[i];
    }
    return mean / float(np);
}


void centering(vec3* points, vec3& mean, int np) {
    for(int i = 0; i < np; i++) {
        points[i] -= mean;
    }
}


void scaling(vec3* points, vec3& mean, int np) {
    float max_dis = 0;

    for(int i = 0; i < np; i++) {
        float dis = (points[i] - mean).length();
        if(dis > max_dis) max_dis = dis;
    }

    for(int i = 0; i < np; i++)
    (*points)[i] /= max_dis;
}


#endif /* OBJ_PARSER_H */
