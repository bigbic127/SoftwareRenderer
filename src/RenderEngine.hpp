#pragma once

#include "RenderWindowSDL.hpp"
#include "Camera.hpp"
#include "Cube.hpp"
#include "Vector.hpp"

class RenderEngine : RenderWindowSDL
{
    public:
        RenderEngine(int w = 800, int h = 600);

        void LoopEvent();
        void Ready();
        void Update();
        void Render();

    private:
        Camera camera;
        Cube cube;

        Vector2D* projectPoints;

        static const int FPS = 200;
        float frameSecond = 1000/FPS;
        float PreviousFrameSecond = 0.0f;
};

