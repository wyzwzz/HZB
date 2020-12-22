//
// Created by wyz on 20-12-6.
//

#ifndef HIERARCHICAL_Z_BUFFER_ZBUFFER_H
#define HIERARCHICAL_Z_BUFFER_ZBUFFER_H

#include "Triangle.h"
#include "Util.h"
#include <glm/glm.hpp>
#include <limits>
#include <map>
#include <memory>
#include <vector>
class Bound2
{
  public:
    Bound2(const glm::vec2 &min_, const glm::vec2 &max_) : min_point(min_), max_point(max_){};
    Bound2()
    {
        float min_ = std::numeric_limits<float>::lowest();
        float max_ = std::numeric_limits<float>::max();
        min_point = {max_, max_};
        max_point = {min_, min_};
    };
    Bound2(const Triangle &tri)
    {
        auto &v = tri.getVertices();
        min_point.x = std::min(v[0].x, std::min(v[1].x, v[2].x));
        min_point.y = std::min(v[0].y, std::min(v[1].y, v[2].y));
        max_point.x = std::max(v[0].x, std::max(v[1].x, v[2].x));
        max_point.y = std::max(v[0].y, std::max(v[1].y, v[2].y));
    }
    bool containing(const Bound2 &bound) const
    {
        if (bound.max_point.x <= this->max_point.x && bound.max_point.y <= this->max_point.y &&
            bound.min_point.x >= this->min_point.x && bound.min_point.y >= this->min_point.y)
            return true;
        else
            return false;
    }
    bool contained(const Bound2 &bound) const
    {
        return bound.containing(*this);
    }
    bool intersect(const Bound2 &bound) const;
    void union_(const Bound2 &bound)
    {
        min_point.x = std::min(min_point.x, bound.min_point.x);
        min_point.y = std::min(min_point.y, bound.min_point.y);
        max_point.x = std::max(max_point.x, bound.max_point.x);
        max_point.y = std::max(max_point.y, bound.max_point.y);
    }
    friend std::ostream &operator<<(std::ostream &os, const Bound2 &b)
    {
        os << "min_point: " << b.min_point.x << " " << b.min_point.y << "\nmax_point: " << b.max_point.x << " "
           << b.max_point.y;
        return os;
    }
    glm::vec2 min_point, max_point;
};
inline Bound2 Union(const Bound2 &b1, const Bound2 &b2, const Bound2 &b3, const Bound2 &b4)
{
    float min_x = min4(b1.min_point.x, b2.min_point.x, b3.min_point.x, b4.min_point.x);
    float min_y = min4(b1.min_point.y, b2.min_point.y, b3.min_point.y, b4.min_point.y);
    float max_x = max4(b1.max_point.x, b2.max_point.x, b3.max_point.x, b4.max_point.x);
    float max_y = max4(b1.max_point.y, b2.max_point.y, b3.max_point.y, b4.max_point.y);
    return {{min_x, min_y}, {max_x, max_y}};
}
inline Bound2 Union(const Bound2 &b1, const Bound2 &b2)
{
    float min_x = std::min(b1.min_point.x, b2.min_point.x);
    float min_y = std::min(b1.min_point.y, b2.min_point.y);
    float max_x = std::max(b1.max_point.x, b2.max_point.x);
    float max_y = std::max(b1.max_point.y, b2.max_point.y);
    return {{min_x, min_y}, {max_x, max_y}};
}
class QuadNode
{
  public:
    QuadNode(const glm::vec2 &min_, const glm::vec2 &max_)
        : childs({nullptr, nullptr, nullptr, nullptr}), parent(nullptr), bound(Bound2(min_, max_)), z_depth(0.f){};
    QuadNode(QuadNode *n1, QuadNode *n2, QuadNode *n3, QuadNode *n4)
    {
        if (n1 != nullptr)
        {
            n1->parent = this;
            this->bound.union_(n1->bound);
            this->z_depth = std::max(this->z_depth, n1->z_depth);
        }
        if (n2 != nullptr)
        {
            n2->parent = this;
            this->bound.union_(n2->bound);
            this->z_depth = std::max(this->z_depth, n2->z_depth);
        }
        if (n3 != nullptr)
        {
            n3->parent = this;
            this->bound.union_(n3->bound);
            this->z_depth = std::max(this->z_depth, n3->z_depth);
        }
        if (n4 != nullptr)
        {
            n4->parent = this;
            this->bound.union_(n4->bound);
            this->z_depth = std::max(this->z_depth, n4->z_depth);
        }
        this->childs = {n1, n2, n3, n4};
        this->parent = nullptr;
    }
    bool isLeafNode() const
    {
        if (!childs[0] && !childs[1] && !childs[2] && !childs[3])
            return true;
        else
            return false;
    }

    std::array<QuadNode *, 4> childs;
    QuadNode *parent;
    Bound2 bound;
    float z_depth;
    int flag = -1;
};

class ZBuffer
{
  public:
    ZBuffer(uint32_t w, uint32_t h);

    bool ZTest(const Triangle &tri);
    void traverseQuadTree();
    void init();
    void setZBuffer(uint32_t row, uint32_t col, float d);
    void printRootDepth()
    {
        std::cout << "root depth: " << root->z_depth << std::endl;
        int cnt = 0;
        for (int i = 0; i < reserve_nodes.size(); i++)
        {
            for (int j = 0; j < reserve_nodes[i].size(); j++)
                if (reserve_nodes[i][j]->z_depth == 1024.f)
                    cnt++;
        }
        std::cout << cnt << std::endl;
    }

  private:
    void updateZBuffer(const QuadNode *node);
    QuadNode *root;
    std::vector<std::vector<QuadNode *>> reserve_nodes;
};

#endif // HIERARCHICAL_Z_BUFFER_ZBUFFER_H
