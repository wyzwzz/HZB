//
// Created by wyz on 20-12-6.
//

#ifndef HIERARCHICAL_Z_BUFFER_RASTERIZER_H
#define HIERARCHICAL_Z_BUFFER_RASTERIZER_H
#include <functional>
#include <vector>
#include "Triangle.h"
#include "Shader.h"
#include "ZBuffer.h"
class Rasterizer {
public:
    Rasterizer(uint32_t width=800,uint32_t height=600);

    void setModel(glm::mat4 model);
    void setView(glm::mat4 view);
    void setProjection(glm::mat4 projection);
    void raster();
    std::vector<unsigned char> getPixels(){return std::move(pixels);}
private:
    glm::mat4 model,view,projection;
    /**
     * Process each fragment's color
     */
    std::function<glm::vec3(fragment_shader_in)> fragment_shader;
    /**
     * Process each input vertex
     */
    std::function<glm::vec3(vertex_shader_in)> vertex_shader;

    std::unique_ptr<ZBuffer> zbuffer;
    /**
     * Store the final pixels' color (RGBA:0-255)
     */
    std::vector<unsigned char> pixels;

    uint32_t window_w,window_h;
};


#endif //HIERARCHICAL_Z_BUFFER_RASTERIZER_H
