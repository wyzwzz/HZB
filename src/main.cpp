//
// Created by wyz on 20-12-3.
//
#include <iostream>
#include "Displayer.h"
int main()
{
    std::cout<<"Hierarchical Z-Buffer"<<std::endl;
    Displayer displayer(1200,900);
    displayer.addRenderOBJ("./nanosuit/nanosuit.obj");
    displayer.render();
}
