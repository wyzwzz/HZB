cmake_minimum_required(VERSION 3.18)
project(Hierarchical_Z_Buffer)
set(CMAKE_CXX_STANDARD 17)
aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC)

add_executable(Hierarchical_Z_Buffer ${SRC})

target_link_directories(Hierarchical_Z_Buffer PRIVATE ./third_party/sdl2-2.0.3/bin/linux64)
find_package( OpenMP)
if(OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()
target_link_libraries(Hierarchical_Z_Buffer PRIVATE
        SDL2 OpenMP::OpenMP_CXX)

target_include_directories(Hierarchical_Z_Buffer PRIVATE
        ./third_party/sdl2-2.0.3/include ./third_party/tinyobjloader)