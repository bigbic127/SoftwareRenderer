#pragma once
#include "Window.hpp"
#include "Vector.hpp"
#include <vector>

using namespace std;

class Renderer : public Window
{
    public:
        Renderer();

        void Loop();
        void Ready();
        void Update();
        void Render();

        void ProcessInput(SDL_Event& event) override ;

        void DrawClear(uint32_t color);
        void DrawGrid(int intervalW = 10, int intervalH = 10, uint32_t color = 0xFF333333);
        void DrawPixel(int x, int y, uint32_t color);
        void DrawRect(int x, int y, int w, int h, uint32_t color);
        void DrawPoint(int x, int y, int w, int h, uint32_t color);

    private:
        int FPS = 200;
        float frameSecond = 1000/FPS;
        float previousFrameSecond = 0.0f;
        bool bIsLooping = false;
        vector<Vector2> projectionPoints;
};