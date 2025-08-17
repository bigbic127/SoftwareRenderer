#pragma once
#include "Window.hpp"
#include "Vector.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include "Scene.hpp"
#include <vector>

using namespace std;

enum class RenderMode 
{
    Wireframe,   // 와이어프레임 그리기
    FloatData,   // float 값 시각화 (ex. Z-depth, Normal length 등)
    Solid,       // 일반 렌더링 (기본 컬러 또는 텍스처)
    Shader       // 셰이더 기반 조명/컬러 렌더링
};

class Renderer : public Window
{
    public:
        Renderer();

        void Loop();
        void Init();
        void Ready();
        void Update();
        void Render();
        void ProcessInput(SDL_Event& event) override;
        void InputTransform(SDL_Event& event);
        void OpenObjFile();
        void DrawClear(uint32_t color = 0xFF000000);
        void DrawGrid(int intervalW = 10, int intervalH = 10, uint32_t color = 0xFF333333);
        void DrawPixel(int x, int y, uint32_t color);
        void DrawRect(int x, int y, int w, int h, uint32_t color);
        void DrawPoint(int x, int y, int w, int h, uint32_t color);
        void DrawLine(Vector2 a, Vector2 b, uint32_t color = 0xFFFFFFFF);
        void DrawTriangle(vector<Vector3>& p, vector<Vector2>& uv, Mesh& mesh);
        float EdgeFunction(const Vector2& a, const Vector2& b, const Vector2& c);
        uint32_t DrawTexture(float u, float v, Mesh& mesh);
        void SetRenderMode(RenderMode render) {renderMode = render;}
        RenderMode GetRenderMode() const{return renderMode;}
    protected:
        uint32_t ColorToOx(float z);
    private:
        int FPS = 200;
        float frameSecond = 1000/FPS;
        float previousFrameSecond = 0.0f;
        bool bIsLooping = false;
        Scene scene;
        vector<Mesh> meshes;
        Camera camera;
        RenderMode renderMode = RenderMode::Wireframe;
        bool bIsSpace = false;
        bool bIsClicked = false;
        bool bIsMClicked = false;
        Vector2 screenPos;
        Vector3 screenOriPos;
        Vector3 targetPos;
};