//
// Created by wyz on 20-12-9.
//

#ifndef HIERARCHICAL_Z_BUFFER_DISPLAYER_H
#define HIERARCHICAL_Z_BUFFER_DISPLAYER_H

#include<SDL2/SDL.h>
class Displayer {
public:
    Displayer(){
        if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0 ){
            printf("%s - SDL could not initialize! SDL Error: %s\n", __FUNCTION__, SDL_GetError());
        }
//        SDL_Surface *surface=SDL_CreateRGBSurfaceFrom();
//        SDL_Texture *tex=SDL_CreateTextureFromSurface();
    }
    ~Displayer(){
        SDL_DestroyRenderer(renderer);
    }
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};


#endif //HIERARCHICAL_Z_BUFFER_DISPLAYER_H
