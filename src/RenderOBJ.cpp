//
// Created by wyz on 20-12-16.
//
#define TINYOBJLOADER_IMPLEMENTATION

#include "RenderOBJ.h"
#include "Util.h"
#include <exception>
#include <iostream>
#include <tiny_obj_loader.h>
#include <glm/glm.hpp>
#define OBJ_HAVE_NORMAL
#define OBJ_HAVE_TEXCOORD

int offset=0;
RenderOBJ::RenderOBJ(std::string obj_file_path)
{
    START_TIMER
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./nanosuit";

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(obj_file_path, reader_config))
    {
        if (!reader.Error().empty())
        {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty())
    {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto &attrib = reader.GetAttrib();
    auto &shapes = reader.GetShapes();
    auto &materials = reader.GetMaterials();
    //    for(auto& m:materials)
    //        std::cout<<m.name<<std::endl;
    size_t tris_num = 0;
    for (size_t s = 0; s < shapes.size(); s++)
    {
        tris_num += shapes[s].mesh.num_face_vertices.size();
    }
    triangle_list.reserve(tris_num);

    for (size_t s = 0; s < shapes.size(); s++)
    {

        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
        {
            int fv = shapes[s].mesh.num_face_vertices[f];
            if (fv != 3)
            {
                throw std::runtime_error("Only support triangle!");
            }
            Triangle tri;
            for (size_t v = 0; v < fv; v++)
            {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
                tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
                tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
                tri.setVertex(v, vx, vy, vz);
#ifdef OBJ_HAVE_NORMAL
                tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
                tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
                tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
                tri.setNormal(v, nx, ny, nz);
#endif
#ifdef OBJ_HAVE_TEXCOORD
                tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
                tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
                tri.setTexCoord(v, tx, ty);
#endif
#ifdef OBJ_HAVE_COLOR
                tinyobj::real_t red = attrib.colors[3 * idx.vertex_index + 0];
                tinyobj::real_t green = attrib.colors[3 * idx.vertex_index + 1];
                tinyobj::real_t blue = attrib.colors[3 * idx.vertex_index + 2];
#endif
            }
            triangle_list.push_back(tri);
            index_offset += fv;
            //            std::cout<<shapes[s].mesh.material_ids[f]<<std::endl;
        }
    }
    auto model=glm::translate(glm::mat4(1.f),glm::vec3(0.f, 0.f, 3.f * (offset--)));
    for(auto& t:triangle_list){
        auto& v=t.getVertices();
        t.setVertex(0,model*v[0]);
        t.setVertex(1,model*v[1]);
        t.setVertex(2,model*v[2]);
    }
    std::cout << "triangle's num is: " << triangle_list.size() << std::endl;
    std::cout << "finish read obj file: " << obj_file_path << std::endl;
    END_TIMER("Read obj ")
}

std::tuple<const Triangle *, size_t> RenderOBJ::getTriangleList()
{
    return std::make_tuple(triangle_list.data(), triangle_list.size());
}

