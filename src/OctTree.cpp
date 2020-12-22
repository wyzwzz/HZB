//
// Created by wyz on 20-12-6.
//

#include "OctTree.h"

void OctTree::addTriangles(std::tuple<const Triangle *, size_t> &triangles_pack)
{
    auto tris = std::get<0>(triangles_pack);
    auto num = std::get<1>(triangles_pack);
    for (size_t i = 0; i < num; i++)
    {
        all_triangles.push_back(&tris[i]);
    }
    std::cout << "total triangle num is: " << all_triangles.size() << std::endl;
}

void OctTree::buildOctTree()
{
    if(all_triangles.size()==0)
        throw std::runtime_error("build Oct tree's triangles are empty!");

    Bound3 bound;
    for(size_t i=0;i<all_triangles.size();i++){
        bound.union_(all_triangles[i]);
    }
    std::cout<<"bound is:\n"<<bound<<std::endl;
    root=new OctNode(bound);
    root->splitBuild(all_triangles);
}
QuadNode *OctNode::splitBuild(std::vector<const Triangle *> &triangles)
{
    return nullptr;
}
