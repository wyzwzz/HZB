//
// Created by wyz on 21-1-5.
//

#ifndef HIERARCHICAL_Z_BUFFER_SCANZBUFFER_H
#define HIERARCHICAL_Z_BUFFER_SCANZBUFFER_H
#include <vector>
class ScanZBuffer{
  public:
    ScanZBuffer()=default;

    std::vector<uint8_t> &getPixels();
};

#endif // HIERARCHICAL_Z_BUFFER_SCANZBUFFER_H
