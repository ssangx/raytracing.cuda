#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include "vec3.h"


std::pair<int, int> parseFaceIndex (std::string str);


void parseObjByName(const std::string fn,
                    std::vector<vec3>& points,
                    std::vector<vec3>& normals,
                    std::vector<vec3>& idxVertex,
                    std::vector<vec3>& idxNormal);
#endif
