#pragma once

#include <SDL.h>

class RenderWindowSDL
{
    public:
        RenderWindowSDL(int w, int h);
        ~RenderWindowSDL();

        bool Initalization();
        void ProcessInput();

        void DrawClearColorBuffer(uint32_t color);
        void DrawGrid(uint32_t color);
        void DrawRect(int posx, int posy, int width, int height, uint32_t color);
        void DrawPixel(int posx, int posy, uint32_t color);

    protected:
        SDL_Window* window;
        SDL_Renderer* renderer;
        bool bIsLooping = true;
        int windowWidth = 800;
        int windowHeight = 600;
        SDL_Event event;
        uint32_t* colorBuffer = nullptr;
        SDL_Texture* colorBufferTexture = nullptr;
};