//
// Created by wyz on 20-12-6.
//

#include "ZBuffer.h"

ZBuffer::ZBuffer(uint32_t w, uint32_t h)
:w(w),h(h)
{
    START_TIMER
    std::vector<std::vector<QuadNode *>> nodes;
    nodes.resize(h);
    for (int i = 0; i < h; i++)
    {
        nodes[i].resize(w);
        for (int j = 0; j < w; j++)
        {
            glm::vec2 min_{j, i};
            glm::vec2 max_{j + 1, i + 1};
            nodes[i][j] = new QuadNode(min_, max_);
            nodes[i][j]->flag = 0;
        }
    }
    reserve_nodes = nodes; // reserve last level nodes
    std::vector<std::vector<QuadNode *>> n_nodes;
    auto getNodePtr = [&nodes](int row, int col, int h, int w) -> QuadNode * {
        if (row < h && col < w)
            return nodes[row][col];
        else
        {
            //            std::cout<<"return nullptr"<<std::endl;
            return nullptr;
        }
    };
    int flag = 0;
    int o_h = h, o_w = w;
    do
    {
        flag++;
        h = (h + 1) / 2;
        w = (w + 1) / 2;
        n_nodes.resize(h);
        for (int i = 0; i < h; i++)
        {
            n_nodes[i].resize(w);
            for (int j = 0; j < w; j++)
            {
                n_nodes[i][j] =
                    new QuadNode(getNodePtr(i * 2, j * 2, o_h, o_w), getNodePtr(i * 2, j * 2 + 1, o_h, o_w),
                                 getNodePtr(i * 2 + 1, j * 2, o_h, o_w), getNodePtr(i * 2 + 1, j * 2 + 1, o_h, o_w));
                n_nodes[i][j]->flag = flag;
            }
        }
        o_h = h;
        o_w = w;
        nodes.swap(n_nodes);
    } while (h != 1 || w != 1);

    root = nodes[0][0];
    if (root->parent != nullptr)
        throw std::runtime_error("root's parent is not nullptr");
    END_TIMER("ZBuffer build ")
}

std::vector<int> book;
void traverse(QuadNode *node, int level)
{
    if (node == nullptr)
        return;
    book[node->flag]++;
    //    if(node->z_depth!=1024.f)
    //        std::cout<<node->z_depth<<std::endl;
    traverse(node->childs[0], level + 1);
    traverse(node->childs[1], level + 1);
    traverse(node->childs[2], level + 1);
    traverse(node->childs[3], level + 1);
}
void ZBuffer::traverseQuadTree()
{
    START_TIMER
    std::cout << __FUNCTION__ << std::endl;
    book.assign(20, 0);
    traverse(root, 0);
    for (int i = 0; i < book.size(); i++)
        std::cout << "level:" << i << " has " << book[i] << " node" << std::endl;
    END_TIMER("Traverse quad-tree ")
}
bool ZBuffer::ZTest(const Bound2& tri_bound_,float nearest)
{
    auto node = root;

    //    std::cout<<node->bound<<std::endl;
    //    std::cout<<tri_bound<<std::endl;
//    if (!node->bound.containing(tri_bound))
//    {
////                std::cout<<"z-depth passed000"<<std::endl;
//        return true;
//    }
    auto tri_bound=intersection(tri_bound_,glm::vec2(0.f),glm::vec2(w,h));
//    std::cout<<"tri_bound "<<tri_bound<<std::endl;
//    std::cout<<"root bound "<<root->bound<<std::endl;
    if(tri_bound.isEmpty())
        return false;
    while (node != nullptr)
    {
        //        std::cout<<node->z_depth<<std::endl;
        if (node->z_depth < nearest && node->bound.containing(tri_bound))
        { // not pass
            //                    std::cout<<root->z_depth<<" "<<node->z_depth<<" "<<nearest<<std::endl;
            //                    std::cout<<tri_bound<<std::endl;
//                        std::cout<<"z-depth failed! flag is: "<<node->flag<<std::endl;
            return false;
        }
        else
        { // test next level
            int i;
            for (i = 0; i < 4; i++)
            {
                if (node->childs[i] && node->childs[i]->bound.containing(tri_bound))
                {
                    node = node->childs[i];
                    break;
                }
            }
            if (i > 3)
            {
//                std::cout<<"z-depth passed! flag is: "<<node->flag<<std::endl;
                return true;
            }
        }
    }
    throw std::runtime_error("reach out of z-test iteration");
}

/**
 * true represent need to rasterize
 */
bool ZBuffer::ZTest(const Triangle &tri)
{
    Bound2 tri_bound{tri};
    auto &v = tri.getVertices();
    float nearest = std::min(v[0].z, std::min(v[1].z, v[2].z));
    return ZTest(tri_bound,nearest);

#ifdef ONE_ZTEST
    auto node = root;

    //    std::cout<<node->bound<<std::endl;
    //    std::cout<<tri_bound<<std::endl;
    if (!node->bound.containing(tri_bound))
    {
        //        std::cout<<"z-depth passed000"<<std::endl;
        return true;
    }

    while (node != nullptr)
    {
        //        std::cout<<node->z_depth<<std::endl;
        if (node->z_depth < nearest && node->bound.containing(tri_bound))
        { // not pass
            //                    std::cout<<root->z_depth<<" "<<node->z_depth<<" "<<nearest<<std::endl;
            //                    std::cout<<tri_bound<<std::endl;
            //            std::cout<<"z-depth failed! flag is: "<<node->flag<<std::endl;
            return false;
        }
        else
        { // test next level
            int i;
            for (i = 0; i < 4; i++)
            {
                if (node->childs[i] && node->childs[i]->bound.containing(tri_bound))
                {
                    node = node->childs[i];
                    break;
                }
            }
            if (i > 3)
            {
                //                std::cout<<"z-depth passed! flag is: "<<node->flag<<std::endl;
                return true;
            }
        }
    }
    throw std::runtime_error("reach out of z-test iteration");
#endif
}

void ZBuffer::setZBuffer(uint32_t row, uint32_t col, float d)
{

    reserve_nodes[row][col]->z_depth = d;

    updateZBuffer(reserve_nodes[row][col]);
}

void ZBuffer::updateZBuffer(const QuadNode *node)
{
    if (node == nullptr)
        throw std::runtime_error("update z-buffer node is nullptr");
    while (true)
    {
        auto p = node->parent;
        if (p == nullptr)
            break;

        auto &childs = p->childs;
        auto _max = 0.0f;
        if (childs[0])
            _max = std::max(_max, childs[0]->z_depth);
        if (childs[1])
            _max = std::max(_max, childs[1]->z_depth);
        if (childs[2])
            _max = std::max(_max, childs[2]->z_depth);
        if (childs[3])
            _max = std::max(_max, childs[3]->z_depth);
        p->z_depth = _max;

        node = p;
    }
}

void recursive_init(QuadNode *node)
{
    if (!node)
        return;
    node->z_depth = 1024.f;
    recursive_init(node->childs[0]);
    recursive_init(node->childs[1]);
    recursive_init(node->childs[2]);
    recursive_init(node->childs[3]);
}
void ZBuffer::init()
{
    recursive_init(root);
}
