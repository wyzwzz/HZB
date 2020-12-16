cmake_minimum_required(VERSION 3.18)
project(Hierarchical_Z_Buffer)
set(CMAKE_CXX_STANDARD 14)
aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC)

add_executable(Hierarchical_Z_Buffer ${SRC})

target_link_directories(Hierarchical_Z_Buffer PRIVATE ./third_party/sdl2-2.0.3/bin/linux64)

target_link_libraries(Hierarchical_Z_Buffer PRIVATE
        SDL2)

target_include_directories(Hierarchical_Z_Buffer PRIVATE
        ./third_party/sdl2-2.0.3/include ./third_party/tinyobjloader)