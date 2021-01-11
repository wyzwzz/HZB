//
// Created by wyz on 20-12-9.
//

#ifndef HIERARCHICAL_Z_BUFFER_DISPLAYER_H
#define HIERARCHICAL_Z_BUFFER_DISPLAYER_H

#include "Camera.h"
#include "Rasterizer.h"
#include "RenderOBJ.h"
#include "Util.h"
#include <SDL.h>
#include <memory>
#include <stdexcept>

/**
 * @brief Using SDL2 to create window and display
 */
class Displayer
{
  public:
    Displayer(uint32_t w, uint32_t h) : w(w), h(h)
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
        {
            printf("%s - SDL could not initialize! SDL Error: %s\n", __FUNCTION__, SDL_GetError());
            throw std::runtime_error("SDL could not initialize");
        }
        SDL_EXPR(window = SDL_CreateWindow("Hierarchical Z-Buffer", 100, 100, w, h, SDL_WINDOW_SHOWN));
        SDL_EXPR(renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED));
        SDL_CHECK

        camera = std::make_unique<Camera>(glm::vec3(0.f, 10.0f, 15.0f));

        /**
         * init rasterizer
         * set mode view projection matrix
         */
        rasterizer = std::make_unique<Rasterizer>(w, h);
        float scale = 1.0f;
        rasterizer->setModel(glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale)));
        rasterizer->setView(camera->getViewMatrix());
        rasterizer->setProjection(glm::perspective(glm::radians(camera->getZoom()), (float)w / h, 0.1f, 50.0f));
    }
    ~Displayer()
    {

        SDL_EXPR(SDL_DestroyRenderer(renderer));
        SDL_EXPR(SDL_DestroyWindow(window));
        SDL_EXPR(SDL_Quit());
    }

  public:
    /**
     * @brief load a render object to obj list from an obj file
     * @see class RenderOBJ
     */
    void addRenderOBJ(std::string obj_file_name);

    /**
     * @brief process PollEvent and raster obj in render_obj_list for each frame, update SDL_Texture and present
     */
    void render();

  private:
    SDL_Window *window;
    uint32_t w;
    uint32_t h;
    SDL_Renderer *renderer;

    /**
     * camera for process mouse and keyboard input
     */
    std::unique_ptr<Camera> camera;

    /**
     * list for render object
     * @see class RenderOBJ
     */
    std::vector<std::unique_ptr<RenderOBJ>> render_obj_list;

    /**
     * instance for rastering triangle
     * @see class Rasterizer
     */
    std::unique_ptr<Rasterizer> rasterizer;
};

#endif // HIERARCHICAL_Z_BUFFER_DISPLAYER_H
