#ifndef GRAPHICS__H
#define GRAPHICS__H

#include <SDL.h>
#include <SDL_image.h>
#include "def.h"

struct Graphic
{
    SDL_Window* window;
    SDL_Renderer* renderer;

    void logErrorAndExit(const char* msg, const char* error)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
        SDL_Quit();
        IMG_Quit();
        exit(EXIT_FAILURE);
    }

    void init()
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            logErrorAndExit("SDL_INIT", SDL_GetError());
        }

        window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        if (window == nullptr)
        {
            logErrorAndExit("SDL_CreateWindow", SDL_GetError());
        }

        //Check if both PNG and JPG can init
        int flags = IMG_INIT_PNG | IMG_INIT_JPG;
        if ((IMG_Init(flags) & flags) != flags)
        {
            logErrorAndExit("IMG_INIT", IMG_GetError());
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        //Use this when use computer at school
        //renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));

        if (renderer == nullptr)
        {
            logErrorAndExit("Create Renderer", SDL_GetError());
        }

        //Smooth the image when scaling
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

        //Keep aspect ratio when resizing window
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    }

    SDL_Texture* loadingTexture(const char* filename)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading image... '%s'", filename);

        SDL_Texture* texture = IMG_LoadTexture(renderer, filename);

        if (texture == nullptr)
        {
            logErrorAndExit("Failed to load image", IMG_GetError());
        }

        return texture;
    }

    void prepareScene(SDL_Texture* background)
    {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,background,nullptr,nullptr);
    }

    void presentScene()
    {
        SDL_RenderPresent(renderer);
    }

    void renderTexture(SDL_Texture* texture, int x, int y)
    {
        SDL_Rect dest;
        dest.x = x;
        dest.y = y;

        SDL_QueryTexture(texture, nullptr, nullptr, &dest.w, &dest.h);

        SDL_RenderCopy(renderer, texture, nullptr, &dest);
    }

    void blitRect(SDL_Texture* texture,int x,int y,int w,int h)
    {
        SDL_Rect dest;
        dest.x=x;
        dest.y=y;
        dest.w=w;
        dest.h=h;

        SDL_RenderCopy(renderer,texture,nullptr,&dest);
    }

    void quit()
    {
        IMG_Quit();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
};

#endif // GRAPHICS__H
