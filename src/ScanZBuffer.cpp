//
// Created by wyz on 21-1-5.
//

#include "ScanZBuffer.h"
#include <iostream>
ScanZBuffer::ScanZBuffer(uint32_t w,uint32_t h)
:w(w),h(h),active_polygon_table(nullptr),active_edge_table(nullptr)
{
    init();
}
void ScanZBuffer::init()
{
    triangles.clear();
    classfied_polygon_table.assign(h, nullptr);
    classfied_edge_table.assign(h, nullptr);
    depth_buffer.assign(w,1024.f);
    pixels.assign(w * h * 4, 0.f);
    active_polygon_table= nullptr;
    active_edge_table= nullptr;
}
std::vector<uint8_t> &ScanZBuffer::getPixels()
{
    return this->pixels;
    triangles.clear();
}

void ScanZBuffer::addTriangle(Triangle &&tri)
{
    uint32_t id=triangles.size();
    triangles.emplace_back(tri);
//    std::cout<<"size: "<<triangles.size()<<std::endl;
    auto* polygon=new ClassfiedPolygonTable();
    auto* edge1=new ClassfiedEdgeTable();
    auto* edge2=new ClassfiedEdgeTable();
    auto* edge3=new ClassfiedEdgeTable();
    ClassfiedEdgeTable* edges[3]={edge1,edge2,edge3};
    polygon->id=id;
    auto& v=tri.getVertices();
    glm::vec3 ab=v[1]-v[0];
    glm::vec3 ac=v[2]-v[0];
    glm::vec3 n=glm::cross(ab,ac);
    float d=-glm::dot(n,glm::vec3(v[0]));
    polygon->a=n[0];
    polygon->b=n[1];
    polygon->c=n[2];
    polygon->d=d;
    auto& tri_color=tri.getColors();
    polygon->color={tri_color[0][0],tri_color[0][1],tri_color[0][2]};
    float max_y=std::max(v[0].y,std::max(v[1].y,v[2].y));
    polygon->dy=max_y-std::min(v[0].y,std::min(v[1].y,v[2].y));
    polygon->next=nullptr;
//    polygon->edges[0]=edge1;
//    polygon->edges[1]=edge2;
//    polygon->edges[2]=edge3;
    addClassfiedPolygonTable(polygon,max_y);
    for(int i=0;i<3;i++){
        auto& v1=v[i];
        auto& v2=v[(i+1)%3];
        float x;
        if(v1.y>v2.y)
            x=v1.x;
        else x=v2.x;
        edges[i]->x=x;
        if(v1.y==v2.y)
            edges[i]->dx=0.f;
        else
            edges[i]->dx=(v2.x-v1.x)/(v1.y-v2.y);
        edges[i]->dy=std::fabs(v1.y-v2.y);
        edges[i]->id=id;
        max_y=std::max(v1.y,v2.y);
        edges[i]->next= nullptr;
        addClassfiedEdgeTable(edges[i],max_y);
    }

}

void ScanZBuffer::addClassfiedPolygonTable(ClassfiedPolygonTable* polygon,int y)
{
    if(y>=0 && y<h){
        if(classfied_polygon_table[y]==nullptr){
            classfied_polygon_table[y]=polygon;
        }
        else{
            polygon->next=classfied_polygon_table[y];
            classfied_polygon_table[y]=polygon;
        }
    }
    else{
//        std::cout<<"polygon y: "<<y<<std::endl;
//        throw std::runtime_error("error max y");
    }
}

void ScanZBuffer::addClassfiedEdgeTable(ClassfiedEdgeTable* edge,int y)
{
    if(y>=0 && y<h){
        if(classfied_edge_table[y]==nullptr){
            classfied_edge_table[y]=edge;
        }
        else{
            edge->next=classfied_edge_table[y];
            classfied_edge_table[y]=edge;
        }
    }
    else{
//        std::cout<<"edge y: "<<y<<std::endl;
//        throw std::runtime_error("error max y");
    }
}
template <class T>
void traverseTable(T t)
{
    int cnt=0;
    while(t!=nullptr){
        cnt++;
        t=t->next;
    }
    std::cout<<"Table cnt: "<<cnt<<std::endl;
}
void ScanZBuffer::scaningRaster()
{
    if(active_polygon_table || active_edge_table)
        throw std::runtime_error("active table is not nullptr");
    // scaning from top to bottom
    for(int cur_h=this->h-1;cur_h>=0;cur_h--){
        depth_buffer.assign(w,1024.f);
        //search classfied_polygon_table for new polygon and put into active_polygon_table
        if(classfied_polygon_table[cur_h]!=nullptr){
//            std::cout<<"cur_h: "<<cur_h<<std::endl;
            auto p=classfied_polygon_table[cur_h];
            while(p!=nullptr){
                //add this polygon to active_polygon_table
                addActivePolygonTable(p);
                //add this polygon'edge which is intersect with the current scan-line to the active_edge_table
                if(classfied_edge_table[cur_h]==nullptr)
                    throw std::runtime_error("classfied_edge_table[cur_h] is nullptr");
                auto p_edge=classfied_edge_table[cur_h];
                ClassfiedEdgeTable* left=nullptr,*right=nullptr;
                int cnt=0;
                while(p_edge!=nullptr && cnt<2){
                    if(p_edge->id==p->id){
                        if(cnt==0)
                            left=p_edge;
                        else if(cnt==1)
                            right=p_edge;
                        else{//for k=0
                            std::cout<<p_edge->dy<<std::endl;
                        }
                        cnt++;
                    }
                    p_edge=p_edge->next;
                }
                if(cnt<2){
                    std::cout<<"cnt: "<<cnt<<std::endl;
                    throw std::runtime_error("cnt < 2");
                }
                if(left->dx>right->dx)
                    std::swap(left,right);
                addActiveEdgeTable(p,left,right,cur_h);
                p=p->next;
            }
        }//endif for search new active polygon
        updateActivePolygonTable();
        auto p_act_edge=active_edge_table;
        while(p_act_edge!=nullptr){
            float zx=p_act_edge->zl;
            for(int x=p_act_edge->xl;x<=p_act_edge->xr;x++){
                if(zx<depth_buffer[x]){
                    depth_buffer[x]=zx;
                    auto idx=(cur_h*h+x)*4;
                    pixels[idx]=p_act_edge->color[0];
                    pixels[idx+1]=p_act_edge->color[1];
                    pixels[idx+2]=p_act_edge->color[2];
                    pixels[idx+3]=0.f;
                }
                zx+=p_act_edge->dzx;
            }
            p_act_edge->dyl-=1;
            p_act_edge->dyr-=1;
            auto id=p_act_edge->id;
            //find new edge from classfied_edge_table if this polygon is still in active_polygon_table
            auto cur_p_act_edge=p_act_edge;
            if(cur_p_act_edge->dyl<0){
//                std::cout<<"update left active edge"<<std::endl;
                p_act_edge=p_act_edge->next;//???
                if(inActivePolygonTable(id)){


                }
                else{
                    deleteActiveEdgeTable(cur_p_act_edge);
                }
                continue;
            }
            if(p_act_edge->dyr<0){
                p_act_edge=p_act_edge->next;
                if(inActivePolygonTable(id)){
                    auto e=findClassfiedEdgeTable(cur_h,id);
                }
                else{
                    deleteActiveEdgeTable(cur_p_act_edge);
                }
                continue;
            }

            p_act_edge->xl+=p_act_edge->dxl;
            p_act_edge->xr+=p_act_edge->dxr;
            p_act_edge->zl+=(p_act_edge->dzx*p_act_edge->dxl+p_act_edge->dzy);
            p_act_edge=p_act_edge->next;
        }


    }//end loop for h rows
    traverseTable<ActivePolygonTable*>(active_polygon_table);
    if(active_polygon_table!=nullptr){
        throw std::runtime_error("final active_polygon_table is not nullptr");
    }

    triangles.clear();
}
void ScanZBuffer::updateActivePolygonTable()
{
//    std::cout<<__FUNCTION__ <<std::endl;
    auto p=active_polygon_table;
    auto head=p,last=p;
    while(p!=nullptr){
        p->dy-=1;
//        std::cout<<p->dy<<std::endl;
        if(p->dy<0){
//            std::cout<<"delete p"<<std::endl;
            if(p==head){
                head=p->next;
//                std::cout<<"delete head p"<<std::endl;
                delete p;
                p=last=head;
            }
            else{
                last->next=p->next;
//                std::cout<<"delete mid p"<<std::endl;
                delete p;
                p=last->next;
            }
        }
        else{
            last=p;
            p=p->next;
        }
    }
    active_polygon_table=head;
}
ActivePolygonTable* ScanZBuffer::findActivePolygonByID(uint32_t id)
{
    if(active_polygon_table== nullptr)
        return nullptr;
    else{
        auto p=active_polygon_table;
        while(p!=nullptr){
            if(p->id==id)
                return p;
            p=p->next;
        }
        return nullptr;
    }
}
void ScanZBuffer::finish()
{
//    std::cout<<"delete polygon"<<std::endl;
    for(auto & i : classfied_polygon_table){
        if(i!= nullptr){
            auto p=i;
            while(p!= nullptr){
//                std::cout<<p<<std::endl;
                auto n_p=p;
                p=p->next;
                delete n_p;
            }
        }
    }
//    std::cout<<"delete edge"<<std::endl;
    for(auto & i : classfied_edge_table){
        if(i!= nullptr){
            auto p=i;
            while(p!= nullptr){
//                std::cout<<p<<std::endl;
                auto n_p=p;
                p=p->next;
                delete n_p;
            }
        }
    }
}
void ScanZBuffer::addActivePolygonTable(ClassfiedPolygonTable *polygon)
{
    auto* act_polygon=new ActivePolygonTable();
    act_polygon->id=polygon->id;
    act_polygon->a=polygon->a;
    act_polygon->b=polygon->b;
    act_polygon->c=polygon->c;
    act_polygon->d=polygon->d;
    act_polygon->dy=polygon->dy;
    act_polygon->next=nullptr;
    if(active_polygon_table==nullptr){
        active_polygon_table=act_polygon;
    }
    else{
        act_polygon->next=active_polygon_table;
        active_polygon_table=act_polygon;
    }
}
void ScanZBuffer::addActiveEdgeTable(ClassfiedPolygonTable* polygon,ClassfiedEdgeTable* left_edge,ClassfiedEdgeTable* right_edge,float y)
{
    if(!left_edge || !right_edge || !polygon)
        throw std::runtime_error("left or right edge or polygon is nullptr");
    ActiveEdgeTable* act_edge_table=new ActiveEdgeTable();
    act_edge_table->xl=left_edge->x;
    act_edge_table->dxl=left_edge->dx;
    act_edge_table->dyl=left_edge->dy;
    act_edge_table->xr=right_edge->x;
    act_edge_table->dxr=right_edge->dx;
    act_edge_table->dyr=right_edge->dy;
    act_edge_table->id=polygon->id;
    act_edge_table->color=polygon->color;
    act_edge_table->next= nullptr;
    if(polygon->c==0)
        throw std::runtime_error("c is zero");
    act_edge_table->zl=-(polygon->d+polygon->a*act_edge_table->xl+polygon->b*y)/polygon->c;
//    std::cout<<act_edge_table->zl<<std::endl;
    act_edge_table->dzx=-polygon->a/polygon->c;
    act_edge_table->dzy=polygon->b/polygon->c;
//    std::cout<<act_edge_table->dzx<<" "<<act_edge_table->dzy<<std::endl;
    if(active_edge_table==nullptr){
        active_edge_table=act_edge_table;
    }
    else{
        act_edge_table->next=active_edge_table;
        active_edge_table=act_edge_table;
    }
}
bool ScanZBuffer::inActivePolygonTable(uint32_t id)
{
    auto p=active_polygon_table;
    while(p!= nullptr){
        if(p->id==id)
            return true;
        p=p->next;
    }
    return false;
}
void ScanZBuffer::deleteActiveEdgeTable(ActiveEdgeTable* edge)
{
    auto head=active_edge_table,p=active_edge_table,last=active_edge_table;
    while(p!= nullptr){
        if(p==edge){
            if(p==head){
                head=head->next;
                delete p;
                break;
            }
            else{
                last->next=p->next;
                delete p;
                break;
            }
        }
        else{
            last=p;
            p=p->next;
        }
    }
    active_edge_table=head;
}
ClassfiedEdgeTable *ScanZBuffer::findClassfiedEdgeTable(int h, uint32_t id)
{
    auto p=classfied_edge_table[h];
    while(p!=nullptr){
        if(p->id==id){
            std::cout<<"find"<<std::endl;
            return p;
        }
        p=p->next;
    }
    throw std::runtime_error("not find classfied_edge_table");
}
