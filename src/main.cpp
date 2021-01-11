//
// Created by wyz on 20-12-3.
//
#include "Displayer.h"
#include <iostream>

bool use_scanline_zbuffer = false;
bool use_hierarchical_zbuffer = false;
bool use_hzb_octtree = false;
int main(int argc, char **argv)
{
    if (argc < 3)
    {
        std::cout << "Usage: Hierarchical_Z_Buffer /path/to/model.obj model_number [-l] [-h] [-o]" << std::endl;
        return 0;
    }
    std::string file_path = argv[1];
    int num = std::stoi(argv[2]);
    if (argc > 4)
    {
        std::cout << "only choose one from -l -h -o" << std::endl;
        return 0;
    }
    if (argc == 4)
    {
        if (std::string(argv[3]) == "-l")
        {
            use_scanline_zbuffer = false;
        }
        else if (std::string(argv[3]) == "-h")
        {
            use_hierarchical_zbuffer = true;
        }
        else if (std::string(argv[3]) == "-o")
        {
            use_hzb_octtree = true;
            use_hierarchical_zbuffer = true;
        }
        else
        {
            std::cout << "Invalid argv! Use default setting" << std::endl;
        }
    }

    Displayer displayer(1024, 1024);

    if (use_hzb_octtree)
    { // for testing oct tree acceleration
        displayer.addRenderOBJ("wall/wall.obj");
    }
    for (size_t i = 0; i < num; i++)
        displayer.addRenderOBJ(file_path);

    displayer.render();

    return 0;
}
