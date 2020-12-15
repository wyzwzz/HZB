//
// Created by wyz on 20-12-9.
//

#include "Displayer.h"





void Displayer::render()
{
    SDL_Event event;
    bool exit=false;
    SDL_Rect rect{0,0,1200,900};
    SDL_Surface* surface1=SDL_LoadBMP("background.bmp");
    SDL_Surface* surface2=SDL_LoadBMP("image.bmp");
    SDL_CHECK
    SDL_Texture* texture1=SDL_CreateTextureFromSurface(renderer,surface1);
    SDL_Texture* texture2=SDL_CreateTextureFromSurface(renderer,surface2);
    SDL_CHECK
    bool t=true;
    auto processEvent=[&exit,&event,&t](){
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
                        case SDLK_n:{
                            t=!t;
                            std::cout<<t<<std::endl;
                        }break;
                    }
                } break;
                case SDL_MOUSEWHEEL:{

                }break;
                case SDL_MOUSEMOTION:{

                }break;
            }
        }
    };

    /**
     * render loop
     */
    while(!exit){
        processEvent();
        SDL_RenderClear(renderer);
        if(t)
            SDL_RenderCopy(renderer,texture1, nullptr,&rect);
        else
            SDL_RenderCopy(renderer,texture2, nullptr,&rect);
        SDL_RenderPresent(renderer);
    }

}
