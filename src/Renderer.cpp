#include "Renderer.hpp"
#include "Mesh.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "File.hpp"
#include <cmath>

using namespace std;

Renderer::Renderer()
{
    Ready();
    Loop();
}

void Renderer::Loop()
{
    while(bIsLooping)
    {
        Input();
        Update();
        Render();
    }
    Quit();
}

void Renderer::Ready()
{
    if (Initalization())
        bIsLooping = true;
    else
        Quit();
    mesh = Mesh();
    mesh.Sphere();
    LoadObjFile("../obj/bunny.obj", mesh.GetVertices(), mesh.GetIndices());
    mesh.GetTransform().SetScale(Vector3(0.7,0.7,0.7));
    mesh.GetTransform().SetScale(Vector3(10,10,10));

    mesh.GetTransform().SetPosition(Vector3(0,-1,0));

}

void Renderer::Update()
{
    //FPS 프레임 딜레이
    int timeToWait = frameSecond - (SDL_GetTicks() - previousFrameSecond);
    if (timeToWait > 0 && timeToWait <= frameSecond)
        SDL_Delay(timeToWait);
    previousFrameSecond = SDL_GetTicks();
    //카메라 파마리터 변경
    camera.SetLookAt(Vector3(0.f,0.5f, 4.f), Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f));
    camera.SetPerspective(70.f, float(width)/height, 0.1f, 100.f);
    //Mesh Transform 처리
    Transform& transform = mesh.GetTransform();
    Vector3 rot = transform.GetRotation();
    rot.y += 0.05f * frameSecond;
    transform.SetRotation(rot);
}

void Renderer::Render()
{
    DrawClear();
    DrawGrid();
    //DrawPoint(10,10,4,4,0xFFFF0000);
    //메쉬 그리기
    vector<Vector3> vertices = mesh.GetVertices();
    vector<Triangle> triangle = mesh.GetIndices();
    vector<Vector3> worldVertices = mesh.GetWorldVertices();
    vector<uint32_t> colors = mesh.GetColors();
    Vector3 cameraPosition = camera.GetPosition();
    //뷰행렬로 변환
    projectionPoints.clear();
    worldVertices.clear();
    for(Vector3& vertice:vertices)
    {
        //행렬 변환 (world * view * projection) -> clip -> NDC(-1 ~ 1 정규화)
        Vector4 vec4 = Vector4(vertice.x, vertice.y, vertice.z, 1.0f);//Vector4로 변환
        Vector4 v = camera.GetProjectionMatrix() * mesh.GetTransform().GetMatrix() * vec4;//프로젝션행렬까지 변환
        Vector4 w = mesh.GetTransform().GetMatrix() * vec4;//뷰행렬까지 변환 라이팅계산
        Vector3 p = Vector3(v.x/v.w , v.y/v.w, v.z/v.w);//NDC 좌표계로 변환
        //화면 중앙으로 위치
        float screenX = (p.x * 0.5f + 0.5f) * width;
        float screenY = (1.0f - (p.y * 0.5f + 0.5f)) * height;
        float zdepth = p.z * 0.5f + 0.5f;
        Vector3 vertex = Vector3(screenX, screenY, zdepth);
        projectionPoints.push_back(vertex);
        worldVertices.push_back(Vector3(w.x , w.y, w.z));
    }
    //픽셀그리기
    if (renderMode == RenderMode::Solid || renderMode == RenderMode::FloatData || renderMode == RenderMode::Shader)
    {
        //면그리기
        int colorIndex = 0;
        for (Triangle& tri: triangle)
        {
            int a = tri.a;
            int b = tri.b;
            int c = tri.c;
            Vector3 v1 = projectionPoints[a];
            Vector3 v2 = projectionPoints[b];
            Vector3 v3 = projectionPoints[c];
            //backface culling 계산
            Vector3 _a = worldVertices[a];
            Vector3 _b = worldVertices[b];
            Vector3 _c = worldVertices[c];
            Vector3 triCenter = (_a + _b + _c) / 3.0f;
            Vector3 cameraDir = (triCenter - cameraPosition).Normalized();
            Vector3 normal = ((_b-_a).Cross(_c-_a)).Normalized();
            if (normal.Dot(cameraDir) >= 0)
            {
                colorIndex += 1;
                continue;
            }
            //광원 및 색상
            float brightness = max(0.0f, normal.Dot(cameraDir * -1));
            brightness *= 1.0f;
            int gray = static_cast<int>(brightness * 255.0f);
            uint32_t color = 0xFF000000 | (gray << 16) | (gray << 8) | gray;// ARGB  각uint8
            //면 그리기
            DrawTriangle(v1, v2, v3, color);
            if (renderMode == RenderMode::FloatData)
            {
                DrawLine(v1.Vector2i(), v2.Vector2i(), 0xFF333333);
                DrawLine(v2.Vector2i(), v3.Vector2i(), 0xFF333333);
                DrawLine(v1.Vector2i(), v3.Vector2i(), 0xFF333333);
                DrawPoint(v1.x, v1.y, 4,4,0xFFFF0000);
                DrawPoint(v2.x, v2.y, 4,4,0xFFFF0000);
                DrawPoint(v3.x, v3.y, 4,4,0xFFFF0000);
            }
            colorIndex += 1;
        }
    }
    else if (renderMode == RenderMode::Wireframe)
    {
    //선그리기
        for (Triangle& tri: triangle)
        {
            int a = tri.a;
            int b = tri.b;
            int c = tri.c;
            Vector3 v1 = projectionPoints[a];
            Vector3 v2 = projectionPoints[b];
            Vector3 v3 = projectionPoints[c]; 
            DrawLine(v1.Vector2i(), v2.Vector2i(), 0xFF333333);
            DrawLine(v2.Vector2i(), v3.Vector2i(), 0xFF333333);
            DrawLine(v1.Vector2i(), v3.Vector2i(), 0xFF333333);
        }
        //점 그리기
        for (Vector3& point:projectionPoints)
        {
            Vector3& p = point;
            DrawPoint(p.x, p.y, 4,4,0xFFFF0000);
        }
    }
    //텍스쳐에 컬러버퍼에 들어있는 데이터 적용
    SDL_UpdateTexture(colorBufferTexture, nullptr, colorBuffer.get(), (int)(width * sizeof(uint32_t)));
    SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
//인풋
void Renderer::ProcessInput(SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_QUIT:
        bIsLooping = false;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
            bIsLooping = false;
        if (event.key.keysym.sym == SDLK_1)
            SetRenderMode(RenderMode::Wireframe);
        else if (event.key.keysym.sym == SDLK_2)
            SetRenderMode(RenderMode::FloatData);
        else if (event.key.keysym.sym == SDLK_3)
            SetRenderMode(RenderMode::Shader);
        else if (event.key.keysym.sym == SDLK_4)
            SetRenderMode(RenderMode::Solid);
        break;
    case SDL_KEYUP:
        break;
    case SDL_MOUSEBUTTONDOWN:
        break;
    case SDL_MOUSEWHEEL:
        break;
    default:
        break;
    }
}
// 화면 초기화
void Renderer::DrawClear(uint32_t color)
{
    for (int y = 0 ; y < height ; y++)
    {
        for (int x = 0 ; x <width ; x++)
        {
            colorBuffer[(width * y) + x] = color;
            zBuffer[(width * y) + x] = numeric_limits<float>::max(); //z버퍼 초기화
        }
    }
}
// 그리드 그리기
void Renderer::DrawGrid(int intervalW, int intervalH, uint32_t color)
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            if (w % intervalW == 0 || h % intervalH == 0)
                colorBuffer[(width * h) + w] = color;
        }
    }
}
// 픽셀 그리기
void Renderer::DrawPixel(int x, int y, uint32_t color)
{
    if (x >=0 && x < width && y >= 0 && y < height)
        colorBuffer[(width * y) + x] = color;
}
// 사각형 그리기
void Renderer::DrawRect(int x, int y, int w, int h, uint32_t color)
{
    for (int _y = 0; _y<h; _y++)
    {
        for (int _x = 0; _x<w; _x++)
        {
            int __x = x + (w -_x);
            int __y = y + (h -_y);
            DrawPixel(__x, __y, color);
        }
    }
}
// 점 그리기
void Renderer::DrawPoint(int x, int y, int w, int h, uint32_t color)
{
    for (int _y = 0; _y<h; _y++)
    {
        for (int _x = 0; _x<w; _x++)
        {
            int __x = x + (w/2 -_x);
            int __y = y + (h/2 -_y);
            DrawPixel(__x, __y, color);
        }
    }
}
//선 그리기 Bresenham’s Line Algorithm
void Renderer::DrawLine(Vector2 a, Vector2 b, uint32_t color)
{
    int dx = abs(b.x - a.x);
    int dy = abs(b.y - a.y);
    int sx = (a.x < b.x) ? 1 : -1;
    int sy = (a.y < b.y) ? 1 : -1;
    int err = dx - dy;
    while (true)
    {
        if (a.x >= 0 && a.x < width && a.y >= 0 && a.y < height)
        {
            DrawPoint(a.x, a.y, 1, 1, color);
        }
        if (a.x == b.x && a.y == b.y)
            break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            a.x += sx;
        }
        if (e2 < dx) {
            err += dx;
            a.y += sy;
        }
    }
}
//삼각형 그리기 Barycentric Algorithm
void Renderer::DrawTriangle(Vector3 a, Vector3 b, Vector3 c, uint32_t color)
{
    //2D좌표로 변환
    Vector2 p0 = { a.x, a.y };
    Vector2 p1 = { b.x, b.y };
    Vector2 p2 = { c.x, c.y };
    //AABB 범위 계산
    float minX = max(0.0f, floor(min({ p0.x, p1.x, p2.x })));
    float maxX = min((float)width - 1, ceil(max({ p0.x, p1.x, p2.x })));
    float minY = max(0.0f, floor(min({ p0.y, p1.y, p2.y })));
    float maxY = min((float)height - 1, ceil(max({ p0.y, p1.y, p2.y })));
    float area = EdgeFunction(p0, p1, p2);
    if (area == 0.0f)
        return; //삼각형이 아니면 그리지 않음

    for (int y = (int)minY; y <= (int)maxY; ++y) {
        for (int x = (int)minX; x <= (int)maxX; ++x) {

            Vector2 p = {x + 0.5f, y + 0.5f}; //픽셀 중심 좌표

            float w0 = EdgeFunction(p1, p2, p);
            float w1 = EdgeFunction(p2, p0, p);
            float w2 = EdgeFunction(p0, p1, p);
            //픽셀이 삼각형 내부에 있는지 확인 (모든 barycentric 계수가 양수이면 내부)
            if (w0 >= 0 && w1 >= 0 && w2 >= 0)
            {
                w0 /= area;
                w1 /= area;
                w2 /= area;
                float z = a.z * w0 + b.z * w1 + c.z * w2;// Barycentric interpolation (Z값 보간)
                int index = y * width + x;
                if (z < zBuffer[index])
                {
                    zBuffer[index] = z;
                    //DrawPoint(x, y, 5, 5, color);
                    colorBuffer[index] = color;
                }
            }
        }
    }
}

float Renderer::EdgeFunction(const Vector2& a, const Vector2& b, const Vector2& c)
{
    return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

uint32_t Renderer::ColorToOx(float z)
{
    z = SDL_clamp(z, 0.0f, 1.0f);

    // 가까울수록 빨강 → 멀수록 파랑
    uint8_t r = static_cast<uint8_t>((1.0f - z) * 255);
    uint8_t g = 0;
    uint8_t b = static_cast<uint8_t>(z * 255);

    return 0xFF000000 | (r << 16) | (g << 8) | b;
}

int main(int arg, char** argv)
{
    Renderer renderer = Renderer();
    return 0;
}