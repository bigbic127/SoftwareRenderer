#include "Window.hpp"
#include <limits>

using namespace std;

Window::Window(int w, int h):width(w), height(h)
{
    //컬러버퍼 생성 - 스마트 포인터로 메모리 관리
    colorBuffer = std::make_unique<uint32_t[]>(width * height);
    //z버퍼 생성 및 초기화
    zBuffer = std::make_unique<float[]>(width * height);
    for (int i = 0; i < (width * height); ++i)
        zBuffer[i] = numeric_limits<float>::max();
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
    //IMGUI 초기화
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO&io=ImGui::GetIO();
    ImGui::StyleColorsLight();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);
    return true;
}

//인풋 설정
void Window::Input()
{
    SDL_PollEvent(&event);
    ImGui_ImplSDL2_ProcessEvent(&event);
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
    zBuffer = std::make_unique<float[]>(width * height);
    for (int i = 0; i < (width * height); ++i)
        zBuffer[i] = numeric_limits<float>::max();

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

void Window::DrawGUI()
{
    // ImGui 새 프레임 시작
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    {
        ImGuiIO&io=ImGui::GetIO();
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(350, 100), ImGuiCond_Always);
        ImGui::Begin("Panel", nullptr,
                    ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoTitleBar);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        if (ImGui::Button("wireframe"))
            IRenderMode(0);
        ImGui::SameLine();
        if (ImGui::Button("flat"))
            IRenderMode(1);
        ImGui::SameLine();
        if (ImGui::Button("Solid"))
            IRenderMode(2);
        ImGui::SameLine();
        if (ImGui::Button("Shader"))
            IRenderMode(3);
        if (ImGui::IsWindowHovered())
        {
            io.WantCaptureKeyboard = true;
            io.WantCaptureMouse = true;
        }
        else
        {
            io.WantCaptureKeyboard = false;
            io.WantCaptureMouse = false;
        }
        
                    
        ImGui::End();
    }
    ImGui::Render();
}

//프로그램 종료 - 메모리 해제
void Window::Quit()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(colorBufferTexture);
    SDL_Quit();
}