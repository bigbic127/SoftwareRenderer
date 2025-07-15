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
    Vector3D camPos = camera->GetPosition();
    float fov = camera->GetFov();
    switch (event.type)
    {
    case SDL_QUIT:
        bIsLooping = false;
        break;

    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
            bIsLooping = false;
            
        if (event.key.keysym.sym == SDLK_w)
            camPos.y += 0.05f;
        else if(event.key.keysym.sym == SDLK_s)
            camPos.y -= 0.05f;
        if (event.key.keysym.sym == SDLK_a)
            camPos.x += 0.05f;
        else if(event.key.keysym.sym == SDLK_d)
            camPos.x -= 0.05f;
        if (event.key.keysym.sym == SDLK_z)
            camPos.z += 0.05f;
        else if(event.key.keysym.sym == SDLK_x)
            camPos.z -= 0.05f;
        if (event.key.keysym.sym == SDLK_c)
            fov += 10.0f;
        else if(event.key.keysym.sym == SDLK_v)
            fov -= 10.0f;
        camera->SetPosition(camPos);
        camera->SetFov(fov);
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
        for (int x = 0 ; x < width ; x++)
        {
            int currentX = posx + ((width/2) - x);
            int currentY = posy + ((width/2) - y);
            DrawPixel(currentX, currentY, color);
        }
    }
}

void RenderWindowSDL::DrawPixel(int posx, int posy, uint32_t color)
{
    //화면 중앙으로 위치
    int currentX = posx + (windowWidth/2);
    int currentY = posy + (windowHeight/2);
    if (currentX >= 0 && currentX < windowWidth && currentY >= 0 && currentY < windowHeight)
    {
        colorBuffer[(windowWidth * currentY)+ currentX] = color;
    }
}

void RenderWindowSDL::DrawLine(int x0, int y0, int x1, int y1, uint32_t color)
{
    int x = x1 - x0;
    int y = y1 - y0;

    int length = (abs(x) >= abs(y)) ? abs(x) : abs(y) ;

    float xInc = x/(float)length;
    float yInc = y/(float)length;

    float currentX = x0;
    float currentY = y0; 

    for (int i = 0; i <= length; i++) {
        //DrawPixel(round(currentX), round(currentY), color);
        DrawRect(round(currentX), round(currentY), 1, 1, color);
        currentX += xInc;
        currentY += yInc;
    }
}
