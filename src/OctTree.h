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

    Bound3(float min_x,float min_y,float min_z,
           float max_x,float max_y,float max_z):
    min_point(min_x,min_y,min_z),max_point(max_x,max_y,max_z){}

    Bound3(const Triangle* tri){
        if(tri== nullptr)
            throw std::runtime_error("bound construct tri which is nullptr");
        auto& v=tri->getVertices();
        min_point.x=std::min(v[0].x,std::min(v[1].x,v[2].x));
        min_point.y=std::min(v[0].y,std::min(v[1].y,v[2].y));
        min_point.z=std::min(v[0].z,std::min(v[1].z,v[2].z));
        max_point.x=std::max(v[0].x,std::max(v[1].x,v[2].x));
        max_point.y=std::max(v[0].y,std::max(v[1].y,v[2].y));
        max_point.z=std::max(v[0].z,std::max(v[1].z,v[2].z));
    }

    void union_(const Bound3& b){
        min_point.x=std::min(min_point.x,b.min_point.x);
        min_point.y=std::min(min_point.y,b.min_point.y);
        min_point.z=std::min(min_point.z,b.min_point.z);
        max_point.x=std::max(max_point.x,b.max_point.x);
        max_point.y=std::max(max_point.y,b.max_point.y);
        max_point.z=std::max(max_point.z,b.max_point.z);
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
        min_point.z=min4(min_point.z,v[0].z,v[1].z,v[2].z);
    }

    bool intersect(const Triangle* tri)
    {
        return this->intersect(Bound3(tri));
    }

    bool intersect(const Bound3& b){
        if( std::max(min_point.x,b.min_point.x)>std::min(max_point.x,b.max_point.x) ||
            std::max(min_point.y,b.min_point.y)>std::min(max_point.y,b.max_point.y) ||
            std::max(min_point.z,b.min_point.z)>std::min(max_point.z,b.max_point.z))
            return false;
        return true;
    }

    friend std::ostream &operator<<(std::ostream &os, const Bound3 &b)
    {
        os << "min_point: " << b.min_point.x << " " << b.min_point.y <<" "<<b.min_point.z<< "\nmax_point: " << b.max_point.x << " "
           << b.max_point.y<<" "<<b.max_point.z;
        return os;
    }

    const glm::vec3& getMinP() const {return min_point;}

    const glm::vec3& getMaxP() const {return max_point;}

  private:
    glm::vec3 min_point, max_point;
};

class OctNode
{
  public:
    OctNode() = default;
    OctNode(const Bound3& bound):bound(bound){childs.fill(nullptr);}
    ~OctNode(){}

    void splitBuild(size_t capacity);

    void addTriangles(std::vector<const Triangle*>& tris){
        triangles.insert(triangles.begin(),tris.begin(),tris.end());
    }

    void addTriangle(const Triangle* tri){
//        std::cout<<__FUNCTION__ <<" "<<triangles.size()<<std::endl;
        triangles.push_back(tri);
    }

    bool isLeafNode() const{
        bool is_leaf=true;
        for(int i=0;i<8;i++)
            if(childs[i]!= nullptr)
                is_leaf=false;
        return is_leaf;
    }

    Bound3 bound;
    std::vector<const Triangle *> triangles;
    std::array<OctNode*, 8> childs;
    int flag = -1;
};

/**
 * @brief split space to accelerate
 */
class OctTree
{
  public:
    OctTree() = default;

    void addTriangles(std::tuple<const Triangle *, size_t> &triangles_pack);

    void buildOctTree();

    const OctNode* getRoot(){return root;}

    void printOctTreeInfo();

  private:
    uint32_t capacity=300;//node's max triangle number

    std::vector<const Triangle *> all_triangles;

    OctNode *root;
};

#endif // HIERARCHICAL_Z_BUFFER_OCTTREE_H
