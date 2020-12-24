//
// Created by wyz on 20-12-6.
//

#include "Triangle.h"

Triangle::Triangle(const glm::vec4 &v0, const glm::vec4 &v1, const glm::vec4 &v2) : vertex({v0, v1, v2})
{
}

void Triangle::setVertex(uint8_t index, const glm::vec3 &v)
{
    vertex[index] = glm::vec4(v, 1.0f);
}

void Triangle::setVertex(uint8_t index, float x, float y, float z)
{
    vertex[index] = {x, y, z, 1.0f};
}

const glm::vec4 &Triangle::getVertex(uint8_t index) const
{
    return vertex[index];
}

void Triangle::setNormal(uint8_t index, float x, float y, float z)
{
    normal[index] = {x, y, z};
}

void Triangle::setTexCoord(uint8_t index, float x, float y)
{
    tex_coord[index] = {x, y};
}
void Triangle::setVertex(uint8_t index, const glm::vec4& v)
{
    vertex[index]=v;
}
