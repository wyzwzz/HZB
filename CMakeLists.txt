cmake_minimum_required(VERSION 3.18)
project(Hierarchical_Z_Buffer)

aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC)

add_executable(Hierarchical_Z_Buffer ${SRC})

target_link_libraries(Hierarchical_Z_Buffer PRIVATE
        )