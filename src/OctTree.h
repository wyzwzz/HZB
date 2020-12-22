//
// Created by wyz on 20-12-6.
//

#ifndef HIERARCHICAL_Z_BUFFER_OCTTREE_H
#define HIERARCHICAL_Z_BUFFER_OCTTREE_H

#include "Triangle.h"
#include "ZBuffer.h"
class Bound3
{
  public:
    Bound3()
    {
        float min_ = std::numeric_limits<float>::lowest();
        float max_ = std::numeric_limits<float>::max();
        min_point = {max_, max_, max_};
        max_point = {min_, min_, min_};
    }
    std::tuple<Bound2, float> getBound2AndDepth(const glm::mat4 &mvp);
    void union_(const Bound3& b){

    }
    void union_(const Triangle* tri){
        if(tri== nullptr)
            throw std::runtime_error("bound union tri which is nullptr");
        auto& v=tri->getVertices();
        max_point.x=max4(max_point.x,v[0].x,v[1].x,v[2].x);
        max_point.y=max4(max_point.y,v[0].y,v[1].y,v[2].y);
        max_point.z=max4(max_point.z,v[0].z,v[1].z,v[2].z);
        min_point.x=min4(min_point.x,v[0].x,v[1].x,v[2].x);
        min_point.y=min4(min_point.y,v[0].y,v[1].y,v[2].y);
        min_point.z=max4(min_point.z,v[0].z,v[1].z,v[2].z);
    }
    friend std::ostream &operator<<(std::ostream &os, const Bound3 &b)
    {
        os << "min_point: " << b.min_point.x << " " << b.min_point.y <<" "<<b.min_point.z<< "\nmax_point: " << b.max_point.x << " "
           << b.max_point.y<<" "<<b.max_point.z;
        return os;
    }
  private:
    glm::vec3 min_point, max_point;
};

class OctNode
{
  public:
    OctNode() = default;
    OctNode(const Bound3 bound):bound(bound){}
    QuadNode *splitBuild(std::vector<const Triangle*>& triangles);
    Bound3 bound;
    std::vector<const Triangle *> triangles;
    std::array<OctNode*, 8> childs;
    int flag = -1;
};

class OctTree
{
  public:
    OctTree() = default;
    void addTriangles(std::tuple<const Triangle *, size_t> &triangles_pack);
    void buildOctTree();

  private:
    uint32_t capacity=8;
    std::vector<const Triangle *> all_triangles;
    OctNode *root;
};

#endif // HIERARCHICAL_Z_BUFFER_OCTTREE_H
