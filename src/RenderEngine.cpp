#pragma once

#include "RenderEngine.hpp"

RenderEngine::RenderEngine(int w, int h):RenderWindowSDL(w,h)
{
    Ready();
    LoopEvent();
}

void RenderEngine::LoopEvent()
{
    while(bIsLooping)
    {
        Update();
        Render();
    }
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void RenderEngine::Ready()
{
    if (Initalization())
        bIsLooping = true;

}

void RenderEngine::Update()
{
    ProcessInput();    
}

void RenderEngine::Render()
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_UpdateTexture(colorBufferTexture, nullptr, colorBuffer, (int)(windowWidth *sizeof(uint32_t)));
    SDL_RenderCopy(renderer, colorBufferTexture,NULL, NULL);

    //DrawClearColorBuffer(0xFFFFFF00);
    DrawGrid(0xFF333333);
    DrawRect(300,200,100,200,0XFFFFFFFF);

    SDL_RenderPresent(renderer);
}
