#pragma once

#include <SDL.h>

class RenderWindowSDL
{
    public:
        RenderWindowSDL(int w, int h);
        ~RenderWindowSDL();

        bool Initalization(int w, int h);
        void LoopEvent();
        void Ready();
        void ProcessInput();
        void Update();
        void Render();

        void SetClearColorBuffer(uint32_t color);

    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        bool bIsLooping = true;
        int width = 800;
        int height = 600;
        SDL_Event event;
        uint32_t* colorBuffer = nullptr;
        SDL_Texture* colorBufferTexture = nullptr;
};

RenderWindowSDL::RenderWindowSDL(int w=800, int h=600): width(w), height(h)
{
    if (Initalization(w, h))
    {
        Ready();
        LoopEvent();
    }
}

RenderWindowSDL::~RenderWindowSDL()
{
    if (colorBuffer)
        delete[] colorBuffer;
}

bool RenderWindowSDL::Initalization(int w, int h)
{
    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        SDL_Log("Unable to Initialzation SDL %s\n", SDL_GetError()); 
        return false;
    }

    window = SDL_CreateWindow("SoftRender", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_BORDERLESS);
    if (!window)
    {
        SDL_Log("Failed to Create Window %s\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        SDL_Log("Failed to Create Renderer %s\n", SDL_GetError());
        return false;
    }
    SDL_Log("Surccese Initialization");
    return true;
}


void RenderWindowSDL::LoopEvent()
{
    while(bIsLooping)
    {
        ProcessInput();
        Update();
        Render();
    }
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void RenderWindowSDL::Ready()
{
    colorBuffer = new uint32_t[width*height];    
    colorBufferTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);

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

void RenderWindowSDL::Update()
{

}

void RenderWindowSDL::Render()
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_UpdateTexture(colorBufferTexture, nullptr, colorBuffer, (int)(width *sizeof(uint32_t)));
    SDL_RenderCopy(renderer, colorBufferTexture,NULL, NULL);

    SetClearColorBuffer(0xFFFFFF00);

    SDL_RenderPresent(renderer);
}

void RenderWindowSDL::SetClearColorBuffer(uint32_t color)
{
    for (int y = 0 ; y < height ; y++)
    {
        for (int x = 0 ; x <width ; x++)
        {
            colorBuffer[(width * y) + x] = color;
        }
    }
}
