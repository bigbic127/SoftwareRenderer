#pragma once

#include "RenderWindowSDL.hpp"

RenderWindowSDL::RenderWindowSDL(int w, int h):windowWidth(w), windowHeight(h)
{

}

RenderWindowSDL::~RenderWindowSDL()
{
    if (colorBuffer)
        delete[] colorBuffer;
}

bool RenderWindowSDL::Initalization()
{
    //SDL 초기화.
    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        SDL_Log("Unable to Initialzation SDL %s\n", SDL_GetError()); 
        return false;
    }

    //SDL 윈도우창 생성.
    window = SDL_CreateWindow("SoftRender", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_BORDERLESS);
    if (!window)
    {
        SDL_Log("Failed to Create Window %s\n", SDL_GetError());
        return false;
    }

    //SDL 렌더러 생성.
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        SDL_Log("Failed to Create Renderer %s\n", SDL_GetError());
        return false;
    }
    SDL_Log("Surccese Initialization");

    //텍스쳐를 생성한 후 텍스쳐의 컬러를 지정하는 컬러버퍼변수를 생성.
    colorBufferTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, windowWidth, windowHeight);
    colorBuffer = new uint32_t[windowWidth*windowHeight];    

    return true;
}

void RenderWindowSDL::ProcessInput()
{
    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        bIsLooping = false;
        break;

    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
            bIsLooping = false;
        break;
    
    default:
        break;
    }    
}


void RenderWindowSDL::DrawClearColorBuffer(uint32_t color)
{
    for (int y = 0 ; y < windowHeight ; y++)
    {
        for (int x = 0 ; x <windowWidth ; x++)
        {
            colorBuffer[(windowWidth * y) + x] = color;
        }
    }
}

void RenderWindowSDL::DrawGrid(uint32_t color)
{
    for (int y = 0 ; y < windowHeight ; y++)
    {
        for (int x = 0 ; x <windowWidth ; x++)
        {
            if (y%10 == 0 || x%10 == 0)
                colorBuffer[(windowWidth * y) + x] = color;
        }
    }
}

void RenderWindowSDL::DrawRect(int posx, int posy, int width, int height, uint32_t color)
{
    for (int y = 0 ; y < height ; y++)
    {
        for (int x = 0 ; x< width ; x++)
        {
            int currentX = posx + x;
            int currentY = posy + y;
            DrawPixel(currentX, currentY, color);
        }
    }
}

void RenderWindowSDL::DrawPixel(int posx, int posy, uint32_t color)
{
    if (posx >= 0 && posx < windowWidth && posy >= 0 && posy < windowHeight)
        colorBuffer[(windowWidth* posy)+ posx] = color;
}
