//
// Created by wyz on 20-12-16.
//

#ifndef HIERARCHICAL_Z_BUFFER_RENDEROBJ_H
#define HIERARCHICAL_Z_BUFFER_RENDEROBJ_H
#include "Triangle.h"
#include <string>
#include <tuple>
#include <vector>

/**
 * @brief load triangles from obj file, using third-party: tiny_obj_loader
 */
class RenderOBJ
{
  public:
    RenderOBJ(std::string obj_file_path);

    std::tuple<const Triangle *, size_t> getTriangleList();

    void calNormals() = delete;

  private:
    std::vector<Triangle> triangle_list;
};

#endif // HIERARCHICAL_Z_BUFFER_RENDEROBJ_H
