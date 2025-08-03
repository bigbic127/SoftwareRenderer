#include "Window.hpp"

Window::Window(int w, int h):width(w), height(h)
{
    //컬러버퍼 생성 - 스마트 포인터로 메모리 관리
    colorBuffer = std::make_unique<uint32_t[]>(width * height);
}

//윈도우 초기화
bool Window::Initalization()
{
    //초기화
    if (SDL_Init(SDL_INIT_EVERYTHING))
        return false;
    //윈도우창 생성
    window = SDL_CreateWindow(
        "SoftwareRenderer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_RESIZABLE);
    if (!window)
        return false;
    //렌더러 생성
    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
        return false;
    //텍스쳐 생성 - 컬러버퍼의 값을 적용
    colorBufferTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        width,
        height);
    SDL_Log("Surccese Initialization.\n");
    return true;
}

//인풋 설정
void Window::Input()
{
    SDL_PollEvent(&event);
    ProcessInput(event);
    switch (event.type)
    {
    case SDL_WINDOWEVENT:
        if (event.window.event == SDL_WINDOWEVENT_RESIZED)
        {
            int w = event.window.data1;
            int h = event.window.data2;
            ResizeBuffers(w, h);
        }
        break;
    
    default:
        break;
    }
}

//윈도우 창 크기 조절
void Window::ResizeBuffers(int w, int h)
{
    SDL_Log("Resize : %i , %i",w,h);
    width = w;
    height = h;
    colorBuffer = std::make_unique<uint32_t[]>(width * height);
    
    if (colorBufferTexture) {
        SDL_DestroyTexture(colorBufferTexture);
    }
    colorBufferTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        width,
        height
    );
}

//프로그램 종료 - 메모리 해제
void Window::Quit()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(colorBufferTexture);
    SDL_Quit();
}