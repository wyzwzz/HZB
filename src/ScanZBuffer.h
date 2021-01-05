//
// Created by wyz on 21-1-5.
//

#ifndef HIERARCHICAL_Z_BUFFER_SCANZBUFFER_H
#define HIERARCHICAL_Z_BUFFER_SCANZBUFFER_H
#include <vector>
#include <cstdint>

struct ClassfiedPolygonTable{

};

struct ClassfiedEdgeTable{

};

struct ActivePolygonTable{

};

struct ActiveEdgeTable{

};

class ScanZBuffer{
  public:
    ScanZBuffer(uint32_t w,uint32_t h);

    std::vector<uint8_t> &getPixels();

  private:
    uint32_t w,h;
    std::vector<uint8_t> pixels;

};

#endif // HIERARCHICAL_Z_BUFFER_SCANZBUFFER_H
