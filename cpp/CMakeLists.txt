cmake_minimum_required(VERSION 3.14)
project(cpp)

SET(PROJECT_NAME Raytracer)
project(${PROJECT_NAME})

include_directories(data)
include_directories(images)

include_directories(src)
include_directories(include)
include_directories(external)

add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCE_DIR}/external/pugixml.cpp
        ${PROJECT_SOURCE_DIR}/src/aabb.cpp
        ${PROJECT_SOURCE_DIR}/src/box.cpp
        ${PROJECT_SOURCE_DIR}/src/bvh.cpp
        ${PROJECT_SOURCE_DIR}/src/material.cpp
        ${PROJECT_SOURCE_DIR}/src/mesh.cpp
        ${PROJECT_SOURCE_DIR}/src/objLoader.cpp
        ${PROJECT_SOURCE_DIR}/src/rectangle.cpp
        ${PROJECT_SOURCE_DIR}/src/scene.cpp
        ${PROJECT_SOURCE_DIR}/src/sphere.cpp
        ${PROJECT_SOURCE_DIR}/src/transform.cpp
        ${PROJECT_SOURCE_DIR}/src/triangle.cpp
        ${PROJECT_SOURCE_DIR}/src/xmlLoader.cpp
        main.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-std=c++11")