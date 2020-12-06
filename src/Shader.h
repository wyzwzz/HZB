//
// Created by wyz on 20-12-6.
//

#ifndef HIERARCHICAL_Z_BUFFER_SHADER_H
#define HIERARCHICAL_Z_BUFFER_SHADER_H
#include <glm/glm.hpp>
struct vertex_shader_in{
    glm::vec3 vertex_pos;
    glm::vec3 vertex_normal;
    glm::vec3 vectex_texcoord;
};
struct fragment_shader_in{
    glm::vec3 eye_pos;
    glm::vec3 color;
    glm::vec3 normal;
    glm::vec3 tex_coord;
};

inline glm::vec3 VertexShader(vertex_shader_in& v_in)
{

}
inline glm::vec3 FragmentShader(fragment_shader_in& f_in)
{

}

#endif //HIERARCHICAL_Z_BUFFER_SHADER_H
