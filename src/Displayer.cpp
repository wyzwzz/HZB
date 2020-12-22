//
// Created by wyz on 20-12-9.
//

#include "Displayer.h"

void Displayer::render()
{
    SDL_Event event;
    bool exit = false;
    SDL_Rect rect{0, 0, int(w), int(h)};
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, w, h);

    auto processEvent = [&exit, &event, this]() {
        bool update_view_matrix = false;
        bool update_projection_matrix = false;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT: {
                exit = true;
            }
            break;
            case SDL_KEYDOWN: {
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE: {
                    exit = true;
                }
                break;
                case SDLK_f: {
                    camera->processKeyboardForArgs(Camera::CameraDefinedKey::FASTER);
                }
                break;
                case SDLK_g: {
                    camera->processKeyboardForArgs(Camera::CameraDefinedKey::SLOWER);
                }
                break;
                case SDLK_w: {
                    camera->processMovementByKeyboard(Camera::CameraMoveDirection::FORWARD, 0.1f);
                    update_view_matrix = true;
                }
                break;
                case SDLK_s: {
                    camera->processMovementByKeyboard(Camera::CameraMoveDirection::BACKWARD, 0.1f);
                    update_view_matrix = true;
                }
                break;
                case SDLK_a: {
                    camera->processMovementByKeyboard(Camera::CameraMoveDirection::LEFT, 0.1f);
                    update_view_matrix = true;
                }
                break;
                case SDLK_d: {
                    camera->processMovementByKeyboard(Camera::CameraMoveDirection::RIGHT, 0.1f);
                    update_view_matrix = true;
                }
                break;
                case SDLK_q: {
                    camera->processMovementByKeyboard(Camera::CameraMoveDirection::UP, 0.1f);
                    update_view_matrix = true;
                }
                break;
                case SDLK_e: {
                    camera->processMovementByKeyboard(Camera::CameraMoveDirection::DOWN, 0.1f);
                    update_view_matrix = true;
                }
                break;
                }
            }
            break;
            case SDL_MOUSEWHEEL: {
                camera->processMouseScroll(event.wheel.y);
                update_projection_matrix = true;
            }
            break;
            case SDL_MOUSEMOTION: {
                if (event.button.button == 1)
                {
                    camera->processMouseMovement(event.motion.xrel, -event.motion.yrel);
                    rasterizer->setViewPos(camera->getCameraPos());
                    update_view_matrix = true;
                }
            }
            break;
            }
            if (update_view_matrix)
                rasterizer->setView(camera->getViewMatrix());
            if (update_projection_matrix)
                rasterizer->setProjection(glm::perspective(glm::radians(camera->getZoom()), (float)w / h, 0.1f, 50.0f));
        }
    };

    /**
     * pass all render obj's triangles to resterizer
     */
    for (auto &p : render_obj_list)
    {
        rasterizer->addTriangleList(p->getTriangleList());
    }
    rasterizer->buildSceneQctTree();
    /**
     * render loop
     */
    auto last = std::chrono::steady_clock::now();
    while (!exit)
    {
        processEvent();

        auto current = std::chrono::steady_clock::now();
        auto t = std::chrono::duration_cast<std::chrono::milliseconds>(current - last);
        last = current;
        std::cout << "Cost time: " << t.count() << "ms" << std::endl;

        rasterizer->raster();

        SDL_UpdateTexture(texture, NULL, rasterizer->getPixels().data(), w * 4);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
        SDL_RenderPresent(renderer);
    }
    SDL_EXPR(SDL_DestroyTexture(texture));
}

void Displayer::addRenderOBJ(std::string obj_file_name)
{
    render_obj_list.push_back(std::move(std::make_unique<RenderOBJ>(obj_file_name)));
}
