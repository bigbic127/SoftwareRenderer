#pragma once
#include "Window.hpp"
#include "Vector.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include <vector>

using namespace std;

enum class RenderMode 
{
    Wireframe,   // 와이어프레임 그리기
    FloatData,   // float 값 시각화 (ex. Z-depth, Normal length 등)
    Shader,      // 셰이더 기반 조명/컬러 렌더링
    Solid        // 일반 렌더링 (기본 컬러 또는 텍스처)
};

class Renderer : public Window
{
    public:
        Renderer();

        void Loop();
        void Ready();
        void Update();
        void Render();
        void ProcessInput(SDL_Event& event) override ;
        void DrawClear(uint32_t color = 0xFF000000);
        void DrawGrid(int intervalW = 10, int intervalH = 10, uint32_t color = 0xFF333333);
        void DrawPixel(int x, int y, uint32_t color);
        void DrawRect(int x, int y, int w, int h, uint32_t color);
        void DrawPoint(int x, int y, int w, int h, uint32_t color);
        void DrawLine(Vector2 a, Vector2 b, uint32_t color = 0xFFFFFFFF);
        void DrawTriangle(Vector3 a, Vector3 b, Vector3 c, uint32_t color = 0xFFFFFFFF);
        bool IsInsideTriangle(float u, float v, float w);
        void SetRenderMode(RenderMode render) {renderMode = render;}
        RenderMode GetRenderMode() const{return renderMode;}
        void ClearZBuffer();
    private:
        int FPS = 200;
        float frameSecond = 1000/FPS;
        float previousFrameSecond = 0.0f;
        bool bIsLooping = false;
        vector<Vector3> projectionPoints;
        vector<Vector3> meshes;
        Mesh mesh;
        Camera camera;
        RenderMode renderMode = RenderMode::Wireframe;
        std::unique_ptr<float[]> zBuffer;
};