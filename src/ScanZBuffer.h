//
// Created by wyz on 21-1-5.
//

#ifndef HIERARCHICAL_Z_BUFFER_SCANZBUFFER_H
#define HIERARCHICAL_Z_BUFFER_SCANZBUFFER_H
#include "Triangle.h"
#include <cstdint>
#include <vector>

struct ClassfiedEdgeTable;
struct ClassfiedPolygonTable
{
    ClassfiedPolygonTable() : next(nullptr)
    {
    }
    float a, b, c, d;
    uint32_t id;
    int dy;
    std::array<float, 3> color;
    ClassfiedPolygonTable *next;
};

struct ClassfiedEdgeTable
{
    ClassfiedEdgeTable() : next(nullptr)
    {
    }
    int x;
    float dx;
    int dy;
    uint32_t id;
    ClassfiedEdgeTable *next;
};

struct ActivePolygonTable
{
    float a, b, c, d;
    uint32_t id;
    int dy;
    ActivePolygonTable *next;
};

struct ActiveEdgeTable
{
    float xl;
    float dxl;
    int dyl;
    float xr;
    float dxr;
    int dyr;
    float zl;
    float dzx;
    float dzy;
    uint32_t id;
    std::array<float, 3> color;
    ActiveEdgeTable *next;
};

class ScanZBuffer
{
  public:
    ScanZBuffer(uint32_t w, uint32_t h);

    void init();

    void finish();

    std::vector<uint8_t> &getPixels();

    void addTriangle(Triangle &&tri);

    void scaningRaster();

  private:
    uint32_t w, h;

    void setPixel(int x, int y, std::array<float, 3> &color);

    std::vector<uint8_t> pixels;

    std::vector<float> depth_buffer;

    std::vector<Triangle> triangles;

    void addClassfiedPolygonTable(ClassfiedPolygonTable *, int);

    std::vector<ClassfiedPolygonTable *> classfied_polygon_table;

    void addClassfiedEdgeTable(ClassfiedEdgeTable *, int);

    std::vector<ClassfiedEdgeTable *> classfied_edge_table;

    ActivePolygonTable *findActivePolygonByID(uint32_t id);

    void addActivePolygonTable(ClassfiedPolygonTable *);

    void updateActivePolygonTable();

    bool inActivePolygonTable(uint32_t id);

    ActivePolygonTable *active_polygon_table;

    void addActiveEdgeTable(ClassfiedPolygonTable *, ClassfiedEdgeTable *, ClassfiedEdgeTable *, float y);

    void updateActiveEdgeTable(uint32_t id, ClassfiedEdgeTable *);

    void deleteActiveEdgeTable(ActiveEdgeTable *);

    ClassfiedEdgeTable *findClassfiedEdgeTable(int h, uint32_t id);

    ActiveEdgeTable *active_edge_table;

    int add_cnt;
};

#endif // HIERARCHICAL_Z_BUFFER_SCANZBUFFER_H
