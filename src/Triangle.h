//
// Created by wyz on 20-12-6.
//

#ifndef HIERARCHICAL_Z_BUFFER_TRIANGLE_H
#define HIERARCHICAL_Z_BUFFER_TRIANGLE_H

#include <glm/glm.hpp>
class Triangle {
public:
    Triangle()=default;

    glm::vec4 GetVertex(uint8_t index);

    void SetVertex(uint8_t index,glm::vec4 v);
    void SetColor(uint8_t index,glm::vec3 c);
    void SetNormal(uint8_t index,glm::vec3 n);
    void SetTexCoord(uint8_t index,glm::vec2 t);
private:
    glm::vec4 vertex[3];
    glm::vec3 color[3];
    glm::vec3 normal[3];
    glm::vec2 tex_coord[3];

};


#endif //HIERARCHICAL_Z_BUFFER_TRIANGLE_H
