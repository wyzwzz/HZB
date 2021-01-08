cmake_minimum_required(VERSION 3.17)
project(Hierarchical_Z_Buffer)
set(CMAKE_CXX_STANDARD 17)

set(SIZEOF_VOIDP ${CMAKE_SIZEOF_VOID_P})
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCH_64 TRUE)
    set(PROCESSOR_ARCH "x64")
else()
    set(ARCH_64 FALSE)
    set(PROCESSOR_ARCH "x86")
endif()

if(WIN32)
    if(NOT WINDOWS)
        set(WINDOWS TRUE)
        set(SDL_LIB SDL2 SDL2main)
    endif()
elseif(UNIX AND NOT APPLE)
    if(CMAKE_SYSTEM_NAME MATCHES ".*Linux")
        set(LINUX TRUE)
        set(SDL_LIB SDL2)
        message(STATUS ${ARCH_64} ${LINUX} ${CMAKE_SYSTEM_NAME})
    endif()
endif()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC)

add_executable(Hierarchical_Z_Buffer ${SRC})

set(THIDR_PARTH_DIR ${CMAKE_SOURCE_DIR}/third_party)
#glm
set(GLM_INCLUDE_DIR ${THIDR_PARTH_DIR}/glm)
#sdl
set(SDL_INCLUDE_DIR ${THIDR_PARTH_DIR}/SDL2-2.0.12/include)
#tinyobjloader
set(TINYOBJLOADER_INCLUDE_DIR ${THIDR_PARTH_DIR}/tinyobjloader)
#openmp
find_package(OpenMP)
if(OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

target_link_libraries(Hierarchical_Z_Buffer PRIVATE
        ${SDL_LIB} OpenMP::OpenMP_CXX)

target_include_directories(Hierarchical_Z_Buffer PRIVATE
        ${SDL_INCLUDE_DIR} ${TINYOBJLOADER_INCLUDE_DIR} ${GLM_INCLUDE_DIR})

add_subdirectory(./third_party/SDL2-2.0.12)