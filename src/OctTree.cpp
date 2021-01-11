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
    if (all_triangles.size() == 0)
        throw std::runtime_error("build Oct tree's triangles are empty!");

    Bound3 bound;
    for (size_t i = 0; i < all_triangles.size(); i++)
    {
        bound.union_(all_triangles[i]);
    }
    //    std::cout<<"bound is:\n"<<bound<<std::endl;
    root = new OctNode(bound);
    root->flag = 0;
    root->addTriangles(all_triangles);
    root->splitBuild(this->capacity);
}

size_t node_num = 0;
size_t leaf_node_num = 0;
size_t total_tri_num = 0;
void printOctTreeInfo_(OctNode *node)
{
    if (node == nullptr)
        return;
    node_num++;
    if (node->isLeafNode())
    {
        leaf_node_num++;
        if (node->triangles.size() > 500)
        {
            std::cout << "leaf node tri num" << node->triangles.size() << std::endl;
            throw std::runtime_error("leaf node tri num > 500");
        }
        total_tri_num += node->triangles.size();
    }
    else
    {
        for (int i = 0; i < 8; i++)
            printOctTreeInfo_(node->childs[i]);
    }
}

void OctTree::printOctTreeInfo()
{
    printOctTreeInfo_(root);
    std::cout << __FUNCTION__ << "\n"
              << "node_num: " << node_num << "\n"
              << "leaf_node_num: " << leaf_node_num << "\n"
              << "total_tri_num: " << total_tri_num << std::endl;
}

void OctNode::splitBuild(size_t capacity)
{
    Bound3 bounds[8];
    auto &min_p = bound.getMinP();
    auto &max_p = bound.getMaxP();

    bounds[0] = {min_p.x, min_p.y, min_p.z, (min_p.x + max_p.x) / 2, (min_p.y + max_p.y) / 2, (min_p.z + max_p.z) / 2};
    bounds[1] = {(min_p.x + max_p.x) / 2, min_p.y, min_p.z, max_p.x, (min_p.y + max_p.y) / 2, (min_p.z + max_p.z) / 2};
    bounds[2] = {min_p.x, (min_p.y + max_p.y) / 2, min_p.z, (min_p.x + max_p.x) / 2, max_p.y, (min_p.z + max_p.z) / 2};
    bounds[3] = {(min_p.x + max_p.x) / 2, (min_p.y + max_p.y) / 2, min_p.z, max_p.x, max_p.y, (min_p.z + max_p.z) / 2};
    bounds[4] = {min_p.x, min_p.y, (min_p.z + max_p.z) / 2, (min_p.x + max_p.x) / 2, (min_p.y + max_p.y) / 2, max_p.z};
    bounds[5] = {(min_p.x + max_p.x) / 2, min_p.y, (min_p.z + max_p.z) / 2, max_p.x, (min_p.y + max_p.y) / 2, max_p.z};
    bounds[6] = {min_p.x, (min_p.y + max_p.y) / 2, (min_p.z + max_p.z) / 2, (min_p.x + max_p.x) / 2, max_p.y, max_p.z};
    bounds[7] = {(min_p.x + max_p.x) / 2, (min_p.y + max_p.y) / 2, (min_p.z + max_p.z) / 2, max_p.x, max_p.y, max_p.z};

    for (int i = 0; i < 8; i++)
    {
        this->childs[i] = new OctNode(bounds[i]);
        this->childs[i]->flag = this->flag + 1;
        for (size_t j = 0; j < this->triangles.size(); j++)
        {
            if (this->childs[i]->bound.intersect(triangles[j]))
            {
                this->childs[i]->addTriangle(triangles[j]);
            }
        }
    }
    this->triangles.clear();

    for (int i = 0; i < 8; i++)
    {
        if (this->childs[i] == nullptr)
        {
            std::cout << "child is nullptr" << std::endl;
        }
        if (this->childs[i]->triangles.size() > capacity)
            this->childs[i]->splitBuild(capacity);
    }
}
