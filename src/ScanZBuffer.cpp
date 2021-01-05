//
// Created by wyz on 21-1-5.
//

#include "ScanZBuffer.h"
ScanZBuffer::ScanZBuffer(uint32_t w,uint32_t h)
:w(w),h(h)
{

}
std::vector<uint8_t> &ScanZBuffer::getPixels()
{
    return this->pixels;
}
