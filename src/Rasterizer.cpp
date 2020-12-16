//
// Created by wyz on 20-12-6.
//

#include "Rasterizer.h"


Rasterizer::Rasterizer(uint32_t width, uint32_t height)
:window_w(width),window_h(height)
{

}
void Rasterizer::setModel(const glm::mat4& model)
{
    this->model=model;
}

void Rasterizer::setView(const glm::mat4& view)
{
    this->view=view;
}

void Rasterizer::setProjection(const glm::mat4& projection)
{
    this->projection=projection;
}


void Rasterizer::addTriangleList(std::tuple<const Triangle *, size_t> triangle)
{
    all_triangles.push_back(triangle);
}
void Rasterizer::raster()
{
    auto mvp=projection*view*model;

    for(auto e:all_triangles){
        auto tris=std::get<0>(e);
        auto num=std::get<1>(e);
        for(size_t i=0;i<num;i++){
            Triangle tri=tris[i];
            glm::vec4 v[]={mvp*tris[i].getVertex(0),
                           mvp*tris[i].getVertex(1),
                           mvp*tris[i].getVertex(2)};

            int outer_cnt=0;
            for(auto& p:v){
                p.x/=p.w;
                p.y/=p.w;
                p.z/=p.w;
                if(fabs(p.x)>1.0f || fabs(p.y)>1.0f || fabs(p.z>1.0f)){
                    outer_cnt++;
                }
            }
            if(outer_cnt>2){
                //out of view volume, so no need to rasterize
                continue;
            }

            for(auto& p:v ){
                p.x=0.5f*window_w*(p.x+1.0f);
                p.y=0.5f*window_h*(p.y+1.0f);

            }

            tri.setVertex(0,v[0]);
            tri.setVertex(1,v[1]);
            tri.setVertex(2,v[2]);
            rasterize(std::move(tri));
        }
    }
}

glm::vec3 computeBarycentric2D(float x, float y, const std::array<glm::vec4,3>& v)
{
    float c1 = (x*(v[1].y - v[2].y) + (v[2].x - v[1].x)*y + v[1].x*v[2].y - v[2].x*v[1].y) / (v[0].x*(v[1].y - v[2].y) + (v[2].x - v[1].x)*v[0].y + v[1].x*v[2].y - v[2].x*v[1].y);
    float c2 = (x*(v[2].y - v[0].y) + (v[0].x - v[2].x)*y + v[2].x*v[0].y - v[0].x*v[2].y) / (v[1].x*(v[2].y - v[0].y) + (v[0].x - v[2].x)*v[1].y + v[2].x*v[0].y - v[0].x*v[2].y);
    float c3 = (x*(v[0].y - v[1].y) + (v[1].x - v[0].x)*y + v[0].x*v[1].y - v[1].x*v[0].y) / (v[2].x*(v[0].y - v[1].y) + (v[1].x - v[0].x)*v[2].y + v[0].x*v[1].y - v[1].x*v[0].y);
    return {c1,c2,c3};
}
glm::vec3 interpolate(float alpha, float beta, float gamma, const glm::vec3& vert1, const glm::vec3& vert2, const glm::vec3& vert3, float weight)
{
    return (alpha * vert1 + beta * vert2 + gamma * vert3) / weight;
}

glm::vec2 interpolate(float alpha, float beta, float gamma, const glm::vec2& vert1, const glm::vec2& vert2, const glm::vec2& vert3, float weight)
{
    auto u = (alpha * vert1[0] + beta * vert2[0] + gamma * vert3[0]);
    auto v = (alpha * vert1[1] + beta * vert2[1] + gamma * vert3[1]);

    u /= weight;
    v /= weight;

    return glm::vec2(u, v);
}

void Rasterizer::rasterize(const Triangle &tri)
{
    auto& v=tri.getVertices();

    /**
     * todo: hierarchical z buffer test
     */


    int right=std::max(int(std::max(v[0].x,std::max(v[1].x,v[2].x))+0.5),(int)window_w);
    int top=std::max(int(std::max(v[0].y,std::max(v[1].y,v[2].y))+0.5),(int)window_h);
    int left=std::min(int(std::min(v[0].x,std::min(v[1].x,v[2].x))),0);
    int buttom=std::min(int(std::min(v[0].y,std::min(v[1].y,v[2].y))),0);
    for(int i=buttom;i<top;i++){
        for(int j=left;j<right;j++){

        }
    }
}

std::vector<uint8_t> &Rasterizer::getPixels()
{
    return pixels;
}




