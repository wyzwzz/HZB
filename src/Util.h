//
// Created by wyz on 20-12-6.
//

#ifndef HIERARCHICAL_Z_BUFFER_UTIL_H
#define HIERARCHICAL_Z_BUFFER_UTIL_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include<SDL2/SDL.h>
#include<iostream>
inline glm::mat4 GetModelMatrix()
{

}

inline glm::mat4 GetViewMatrix()
{

}

template<typename T>
inline glm::mat4 GetProjectionMatrix(T fov,T aspect,T z_naer,T z_far)
{
    return glm::perspective(fov,aspect,z_naer,z_far);
}

#define DEBUG
#ifdef DEBUG
#define SDL_EXPR(exec) \
        exec;          \
            {              \
                std::string str=SDL_GetError();           \
                if(str.length()>0){                      \
                    std::cout<<__FILE__<<"  function:"<<__FUNCTION__<<"  line:"<<__LINE__<<"  SDL Error: "<<str<<std::endl;               \
            }              \
        }

#define SDL_CHECK \
        {         \
            std::string str=SDL_GetError(); \
            if(str.length()>0){             \
                std::cout<<"SDL Error: "<<str<<" exist before line "<<__LINE__<< \
                " in file "<<__FILE__<<std::endl;\
            }\
        }

#else
#define SDL_EXPR(exec) exec
#define SCL_CHECK
#endif



#endif //HIERARCHICAL_Z_BUFFER_UTIL_H
