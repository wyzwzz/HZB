//
// Created by wyz on 20-12-9.
//

#include "Displayer.h"





void Displayer::render()
{
    SDL_Event event;
    bool exit=false;
    SDL_Rect rect{0,0,1200,900};
    SDL_Surface* surface=SDL_LoadBMP("background.bmp");
    SDL_CHECK
    SDL_Texture* texture=SDL_CreateTextureFromSurface(renderer,surface);
    SDL_CHECK
    while(!exit){
        while(SDL_PollEvent(&event)){
            switch (event.type) {
                case SDL_QUIT: {
                    exit=true;
                } break;
                case SDL_KEYDOWN:{
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:{
                            exit=true;
                        }break;
                    }
                } break;
            }
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,texture, nullptr,&rect);
        SDL_RenderPresent(renderer);
    }

}
