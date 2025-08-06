#pragma once

#include <SDL.h>
#include <memory>
#include <tuple>
#include "Input.hpp"

class Window
{
public: 
    Window(int w=1280, int h=720);
    bool Initalization();
    void Input();
    void Quit();
    void SetWindowSize(int w, int h) {width = w; height = h;}
    void ResizeBuffers(int w, int h);
    virtual void ProcessInput(SDL_Event& event) = 0;//렌더러 인풋 가상함수
protected:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* colorBufferTexture = nullptr;
    std::unique_ptr<uint32_t[]> colorBuffer;
    std::unique_ptr<float[]> zBuffer;
    SDL_Event event;
    int width;
    int height;
};



