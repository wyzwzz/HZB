//
// Created by wyz on 20-12-6.
//

#include "Rasterizer.h"
#include <iostream>
#include <omp.h>
Rasterizer::Rasterizer(uint32_t width, uint32_t height)
:window_w(width),window_h(height),
 fragment_shader(FragmentShader),vertex_shader(VertexShader),
 zbuffer(std::make_unique<ZBuffer>(width,height))
{
    zbuffer->traverseQuadTree();

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
void printmat4(const glm::mat4& m)
{
    std::cout<<m[0][0]<<" "<<m[0][1]<<" "<<m[0][2]<<" "<<m[0][3]<<std::endl
             <<m[1][0]<<" "<<m[1][1]<<" "<<m[1][2]<<" "<<m[1][3]<<std::endl
             <<m[2][0]<<" "<<m[2][1]<<" "<<m[2][2]<<" "<<m[2][3]<<std::endl
             <<m[3][0]<<" "<<m[3][1]<<" "<<m[3][2]<<" "<<m[3][3]<<std::endl;
}
void Rasterizer::raster()
{
    pixels.assign(window_w*window_h*4,0.f);
    depth_buffer.assign(window_h*window_w,1024.f);
    zbuffer->init();

    float f1=(50.f-0.1f)/2;
    float f2=(50.f+0.1f)/2;

    auto inverse_transform=glm::transpose(glm::inverse(view*model));
    int offset=0;
    for(auto e:all_triangles){
        auto tris=std::get<0>(e);
        auto num=std::get<1>(e);
        model=glm::translate(glm::mat4(1.0f),glm::vec3(0.f,0.f,3.f*offset--));
        auto mvp=projection*view*model;
//#pragma omp parallel for
        for(size_t i=0;i<num;i++){
            Triangle tri=tris[i];
//            tri.setNormal(0,tri.getVertex(0));
//            tri.setNormal(1,tri.getVertex(1));
//            tri.setNormal(2,tri.getVertex(2));
            glm::vec4 v[]={mvp*tris[i].getVertex(0),
                           mvp*tris[i].getVertex(1),
                           mvp*tris[i].getVertex(2)};

            int outer_cnt=0;
            for(auto& p:v){
                p.x/=p.w;//p.w is view space depth-z
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
//            std::cout<<"outer_cnt: "<<outer_cnt<<std::endl;

            glm::vec3 n[3]={
                    inverse_transform*glm::vec4(tri.getNormal(0),0.0f),
                    inverse_transform*glm::vec4(tri.getNormal(1),0.0f),
                    inverse_transform*glm::vec4(tri.getNormal(2),0.0f)
            };

            for(auto& p:v ){
                p.x=0.5f*window_w*(p.x+1.0f);
                p.y=0.5f*window_h*(p.y+1.0f);
                p.z=p.z*f1+f2;
            }

            tri.setVertex(0,v[0]);
            tri.setVertex(1,v[1]);
            tri.setVertex(2,v[2]);
//            tri.setNormal(0,n[0]);
//            tri.setNormal(1,n[1]);
//            tri.setNormal(2,n[2]);
            rasterize(tri);
        }
    }
//    zbuffer->printRootDepth();
}

std::tuple<float,float,float> computeBarycentric2D(float x, float y, const std::array<glm::vec4,3>& v)
{
    float c1 = (x*(v[1].y - v[2].y) + (v[2].x - v[1].x)*y + v[1].x*v[2].y - v[2].x*v[1].y) / (v[0].x*(v[1].y - v[2].y) + (v[2].x - v[1].x)*v[0].y + v[1].x*v[2].y - v[2].x*v[1].y);
    float c2 = (x*(v[2].y - v[0].y) + (v[0].x - v[2].x)*y + v[2].x*v[0].y - v[0].x*v[2].y) / (v[1].x*(v[2].y - v[0].y) + (v[0].x - v[2].x)*v[1].y + v[2].x*v[0].y - v[0].x*v[2].y);
    float c3 = (x*(v[0].y - v[1].y) + (v[1].x - v[0].x)*y + v[0].x*v[1].y - v[1].x*v[0].y) / (v[2].x*(v[0].y - v[1].y) + (v[1].x - v[0].x)*v[2].y + v[0].x*v[1].y - v[1].x*v[0].y);
    return {c1,c2,c3};
}

template<typename T>
T interpolate(float alpha, float beta, float gamma, T& vert1, T& vert2, T& vert3, float weight)
{
    return (alpha * vert1 + beta * vert2 + gamma * vert3) / weight;
}

bool insideTriangle(float x,float y,const std::array<glm::vec4,3>& v)
{
    float c1 = (v[1].x - v[0].x)*(y - v[0].y) - (x - v[0].x)*(v[1].y - v[0].y);
    float c2 = (v[2].x - v[1].x)*(y - v[1].y) - (x - v[1].x)*(v[2].y - v[1].y);
    float c3 = (v[0].x - v[2].x)*(y - v[2].y) - (x - v[2].x)*(v[0].y - v[2].y);
    if ((c1 >= 0 && c2 >= 0 && c3 >= 0) || (c1 < 0 && c2 < 0 && c3 < 0))
        return true;
    else
        return false;
}

void Rasterizer::rasterize(const Triangle &tri)
{
    auto& v=tri.getVertices();
    auto& color=tri.getColors();
    auto& normal=tri.getNormals();
    auto& tex_coord=tri.getTexCoords();
//    std::cout<<normal[0][0]<<" "<<normal[0][1]<<" "<<normal[0][2]<<" "
//            <<normal[1][0]<<" "<<normal[1][1]<<" "<<normal[1][2]<<" "
//            <<normal[2][0]<<" "<<normal[2][1]<<" "<<normal[2][2]<<std::endl;
    /**
     * todo: hierarchical z buffer test
     */
#define ZTEST
#ifdef ZTEST

    if(!zbuffer->ZTest(tri))
        return;
#endif
    /**
     * first get the triangle's bounding box
     * second get the intersection of bounding box with the viewport window
     * make sure the fragment is valid
     */
    int right=std::min(int(std::max(v[0].x,std::max(v[1].x,v[2].x))+0.5),(int)window_w);
    int top=std::min(int(std::max(v[0].y,std::max(v[1].y,v[2].y))+0.5),(int)window_h);
    int left=std::max(int(std::min(v[0].x,std::min(v[1].x,v[2].x))),0);
    int bottom=std::max(int(std::min(v[0].y,std::min(v[1].y,v[2].y))),0);
//#pragma omp parallel for
    for(int i=bottom;i<top;i++){
        for(int j=left;j<right;j++){

            if(insideTriangle(j+0.5f,i+0.5f,v)){
                auto[alpha,beta,gamma]=computeBarycentric2D(j+0.5,i+0.5,v);
                float interpolated_view_space_z = 1.0 / (alpha / v[0].w + beta / v[1].w + gamma / v[2].w);
                float zp = alpha * v[0].z / v[0].w + beta * v[1].z / v[1].w + gamma * v[2].z / v[2].w;
                zp *= interpolated_view_space_z;
                if(zp<0.1f || zp>50.f)
                    continue;
                /**
                 * z-buffer test
                 */
                if(zp<depth_buffer[(window_h-1-i)*window_w+j]){
                    float modify_alpha=alpha/v[0].w;
                    float modify_beta=beta/v[1].w;
                    float modify_gamma=gamma/v[2].w;
                    glm::vec3 interpolated_normal=(modify_alpha*normal[0]+modify_beta*normal[1]+modify_gamma*normal[2])*interpolated_view_space_z;

                    glm::vec2 interpolated_texcoord=(modify_alpha*tex_coord[0]+modify_beta*tex_coord[1]+modify_gamma*tex_coord[2])*interpolated_view_space_z;

                    auto pixel_color=fragment_shader({view_pos,interpolated_normal,interpolated_texcoord});
                    setPixel(j,i,pixel_color);
                    depth_buffer[(window_h-1-i)*window_w+j]=zp;
#ifdef ZTEST
                    zbuffer->setZBuffer(i,j,zp);
#endif
                }
            }
        }
    }
}

std::vector<uint8_t> &Rasterizer::getPixels()
{
    return pixels;
}

void Rasterizer::setViewPos(const glm::vec3& view_pos)
{
    this->view_pos=view_pos;
}

void Rasterizer::setPixel(int x, int y, glm::vec4 pixel_color)
{
    uint32_t idx=4*((window_h-1-y)*window_w+x);
    pixels[idx]=pixel_color.r*255;
    pixels[idx+1]=pixel_color.g*255;
    pixels[idx+2]=pixel_color.b*255;
    pixels[idx+3]=pixel_color.a*255;
}




