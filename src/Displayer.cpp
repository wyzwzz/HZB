//
// Created by wyz on 20-12-9.
//

#include "Displayer.h"


void Displayer::render()
{
    SDL_Event event;
    bool exit=false;
    SDL_Rect rect{0,0,1200,900};
    SDL_Surface* surface1=SDL_LoadBMP("./test_image/background.bmp");
    SDL_Surface* surface2=SDL_LoadBMP("./test_image/image.bmp");
    SDL_CHECK
    SDL_Texture* texture1=SDL_CreateTextureFromSurface(renderer,surface1);
    SDL_Texture* texture2=SDL_CreateTextureFromSurface(renderer,surface2);
    SDL_CHECK
    bool t=true;
    auto processEvent=[&exit,&event,&t,this](){
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
                        case SDLK_f:{
                            camera->processKeyboardForArgs(Camera::CameraDefinedKey::FASTER)
                        }break;
                        case SDLK_g:{
                            camera->processKeyboardForArgs(Camera::CameraDefinedKey::SLOWER);
                        }break;
                    }
                } break;
                case SDL_MOUSEWHEEL:{
                    camera->processMouseScroll(event.wheel.y);
                }break;
                case SDL_MOUSEMOTION:{
                    camera->processMouseMovement(event.motion.xrel,
                                                 event.motion.yrel);
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

void Displayer::addRenderOBJ(std::string obj_file_name)
{
    render_obj_list.push_back(std::move(std::make_unique<RenderOBJ>(obj_file_name)));
}
