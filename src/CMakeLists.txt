cmake_minimum_required(VERSION 3.10)

project(RayMarching)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/)

add_executable(RayMarching 
    main/raymarching.cxx
    classes/Sphere.cxx
    )
target_link_libraries(RayMarching ${SDL2_LIBRARIES})