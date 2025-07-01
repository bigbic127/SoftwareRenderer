#include "RenderWindowSDL.hpp"

class RenderEngine : RenderWindowSDL
{
    public:
        RenderEngine(int w = 800, int h = 600);

        void LoopEvent();
        void Ready();
        void Update();
        void Render();

};

