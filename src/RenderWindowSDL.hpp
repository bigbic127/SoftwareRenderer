#pragma once

#include <SDL.h>
#include "Camera.hpp"

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
        void DrawLine(int sx, int sy, int ex, int ey, uint32_t color);
        void DrawFill(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
        void DrawFillBottom(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
        void DrawFillTop(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

        void SetCamera(Camera* cam) {camera = cam;}

    protected:
        SDL_Window* window;
        SDL_Renderer* renderer;
        bool bIsLooping = true;
        int windowWidth = 800;
        int windowHeight = 600;
        SDL_Event event;
        uint32_t* colorBuffer = nullptr;
        SDL_Texture* colorBufferTexture = nullptr;
        Camera* camera;
};