//
// Created by wyz on 20-12-6.
//

#ifndef HIERARCHICAL_Z_BUFFER_OCTTREE_H
#define HIERARCHICAL_Z_BUFFER_OCTTREE_H

#include "Triangle.h"
#include "ZBuffer.h"
class Bound3{
public:
    Bound3(){
        float min_=std::numeric_limits<float>::lowest();
        float max_=std::numeric_limits<float>::max();
        min_point={max_,max_,max_};
        max_point={min_,min_,min_};
    }
    std::tuple<Bound2,float> getBound2AndDepth(const glm::mat4& mvp);

private:
    glm::vec3 min_point,max_point;
};

class OctNode{
public:
    OctNode()=default;

    Bound3 bound;
    std::vector<Triangle*> triangles;
    std::array<Triangle*,8> childs;
    int flag=-1;
};

class OctTree {
public:
    OctTree()=default;
    void addTriangles(std::tuple<const Triangle*,size_t>& triangles_pack);
    void buildQctTree();
private:
    QuadNode* splitBuild();
private:
    std::vector<Triangle*> all_triangles;
    QuadNode* root;
};


#endif //HIERARCHICAL_Z_BUFFER_OCTTREE_H
