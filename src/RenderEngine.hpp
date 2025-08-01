#pragma once

#include "RenderWindowSDL.hpp"
#include "Camera.hpp"
#include "Cube.hpp"
#include "Mesh.hpp"
#include "Vector.hpp"

#include <vector>

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
        Mesh mesh;

        Vector2D* projectPoints;

        static const int FPS = 200;
        float frameSecond = 1000/FPS;
        float PreviousFrameSecond = 0.0f;
};

