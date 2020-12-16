//
// Created by wyz on 20-12-6.
//

#ifndef HIERARCHICAL_Z_BUFFER_TRIANGLE_H
#define HIERARCHICAL_Z_BUFFER_TRIANGLE_H

#include <glm/glm.hpp>
#include <array>
class Triangle {
public:
    Triangle()=default;
    Triangle(const glm::vec4& v0,const glm::vec4& v1,const glm::vec4& v2);


    const glm::vec4& getVertex  (uint8_t index) const;
    const std::array<glm::vec4,3>& getVertices() const{return vertex;}
    void setVertex(uint8_t index,const glm::vec3& v);
    void setVertex(uint8_t index,float x,float y,float z);
    void setColor(uint8_t index,const glm::vec3& c);
    void setNormal(uint8_t index,const glm::vec3& n);
    void setNormal(uint8_t index,float x,float y,float z);
    void setTexCoord(uint8_t index,const glm::vec2& t);
    void setTexCoord(uint8_t index,float x,float y);
private:
    std::array<glm::vec4,3> vertex;
    std::array<glm::vec3,3> color;
    std::array<glm::vec3,3> normal;
    std::array<glm::vec2,3> tex_coord;

};


#endif //HIERARCHICAL_Z_BUFFER_TRIANGLE_H
