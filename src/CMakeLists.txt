cmake_minimum_required(VERSION 3.10)

project(RayMarching)

set(SDL2_DIR SDL2)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/../include)

message(${SDL2_INCLUDE_DIRS})
message(${SDL2_LIBRARIES})

add_executable(RayMarching 
    main/raymarching.cxx
    classes/Sphere.cxx
    classes/Cube.cxx
    classes/Vec3.cxx
    classes/CombinedShape.cxx
    classes/InfiniSphere.cxx
    )

target_link_libraries(RayMarching ${SDL2_LIBRARIES})
