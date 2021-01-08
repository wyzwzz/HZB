//
// Created by wyz on 20-12-3.
//
#include "Displayer.h"
#include <iostream>
int main(int argc,char** argv)
{
    std::cout << "Hierarchical Z-Buffer" << std::endl;
    Displayer displayer(1024, 1024);
//    displayer.addRenderOBJ("./wall/wall.obj");
    displayer.addRenderOBJ("./nanosuit/nanosuit.obj");
//    displayer.addRenderOBJ("./nanosuit/nanosuit.obj");
//    displayer.addRenderOBJ("./nanosuit/nanosuit.obj");
//    displayer.addRenderOBJ("./nanosuit/nanosuit.obj");
//    displayer.addRenderOBJ("./nanosuit/nanosuit.obj");
//    displayer.addRenderOBJ("./nanosuit/nanosuit.obj");
//    displayer.addRenderOBJ("./nanosuit/nanosuit.obj");
//    displayer.addRenderOBJ("./nanosuit/nanosuit.obj");
//    displayer.addRenderOBJ("./nanosuit/nanosuit.obj");
//    displayer.addRenderOBJ("./nanosuit/nanosuit.obj");
//    displayer.addRenderOBJ("./nanosuit/nanosuit.obj");
//    displayer.addRenderOBJ("./nanosuit/nanosuit.obj");
//    displayer.addRenderOBJ("./nanosuit/nanosuit.obj");
//    displayer.addRenderOBJ("./nanosuit/nanosuit.obj");
//    displayer.addRenderOBJ("./nanosuit/nanosuit.obj");
//    displayer.addRenderOBJ("./nanosuit/nanosuit.obj");
    displayer.render();
    return 0;
}
