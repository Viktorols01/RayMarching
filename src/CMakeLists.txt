cmake_minimum_required(VERSION 3.10)

project(RayMarching)

#message(${SDL2_INCLUDE_DIRS})
#message(${SDL2_LIBRARIES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

include_directories(${CMAKE_SOURCE_DIR}/../include)
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

add_executable(RayMarching 
    main/raymarching.cxx
    classes/Sphere.cxx
    classes/Cube.cxx
    classes/Vec3.cxx
    classes/CombinedShape.cxx
    classes/InfiniSphere.cxx
    )

add_executable(Hello 
    EXCLUDE_FROM_ALL
    main/hello.c
    )

target_link_libraries(RayMarching PRIVATE SDL3::SDL3)

target_link_libraries(Hello PRIVATE SDL3::SDL3)