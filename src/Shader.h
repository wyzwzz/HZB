//
// Created by wyz on 20-12-6.
//

#ifndef HIERARCHICAL_Z_BUFFER_SHADER_H
#define HIERARCHICAL_Z_BUFFER_SHADER_H
#include <glm/glm.hpp>
struct vertex_shader_in
{
    glm::vec3 vertex_pos;
    glm::vec3 vertex_normal;
    glm::vec3 vectex_texcoord;
};
struct fragment_shader_in
{
    glm::vec3 eye_pos;
    glm::vec3 normal;
    glm::vec2 tex_coord;
};

inline glm::vec4 VertexShader(vertex_shader_in v_in)
{
    return {};
}
inline glm::vec4 FragmentShader(fragment_shader_in f_in)
{
    return glm::vec4(f_in.normal, 1.0f);
}

#endif // HIERARCHICAL_Z_BUFFER_SHADER_H
