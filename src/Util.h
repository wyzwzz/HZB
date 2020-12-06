//
// Created by wyz on 20-12-6.
//

#ifndef HIERARCHICAL_Z_BUFFER_UTIL_H
#define HIERARCHICAL_Z_BUFFER_UTIL_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
glm::mat4 GetModelMatrix()
{

}

glm::mat4 GetViewMatrix()
{

}

template<typename T>
glm::mat4 GetProjectionMatrix(T fov,T aspect,T z_naer,T z_far)
{
    return glm::perspective(fov,aspect,z_naer,z_far);
}
#endif //HIERARCHICAL_Z_BUFFER_UTIL_H
