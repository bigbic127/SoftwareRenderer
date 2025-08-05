#include "Renderer.hpp"
#include "Mesh.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "cmath"

using namespace std;

Renderer::Renderer()
{
    zBuffer = std::make_unique<float[]>(width * height);
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

}

void Renderer::Update()
{
    //FPS 프레임 딜레이
    int timeToWait = frameSecond - (SDL_GetTicks() - previousFrameSecond);
    if (timeToWait > 0 && timeToWait <= frameSecond)
        SDL_Delay(timeToWait);
    previousFrameSecond = SDL_GetTicks();

    //카메라 파마리터 변경
    camera.SetLookAt(Vector3(0.f,4.f, 4.f), Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f));
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
    Transform transform = mesh.GetTransform();
    Matrix4x4 meshPoint = transform.GetMatrix();
    Matrix4x4 camPoint = camera.GetMatrix();
    projectionPoints.clear();
    worldVertices.clear();
    for(Vector3& vertice:vertices)
    {
        //행렬 변환 (world * view * projection) -> clip -> NDC(-1 ~ 1 정규화)
        Vector3 p = camPoint * meshPoint * vertice;
        //화면 중앙으로 위치
        float screenX = (p.x * 0.5f + 0.5f) * width;
        float screenY = (1.0f - (p.y * 0.5f + 0.5f)) * height;
        Vector2 vertex = Vector2(screenX, screenY);
        projectionPoints.push_back(vertex);
        worldVertices.push_back(p);
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
            //backface culling 계산
            Vector3 _a = worldVertices[a];
            Vector3 _b = worldVertices[b];
            Vector3 _c = worldVertices[c];
            Vector3 normal = ((_c-_a).Cross(_b-_a)).Normalized();
            if (normal.Dot(cameraPosition) >= 0)
            {
                colorIndex += 1;
                continue;
            }
            Vector2 v1 = projectionPoints[a];
            Vector2 v2 = projectionPoints[b];
            Vector2 v3 = projectionPoints[c];
            DrawTriangle(v1, v2, v3, colors[colorIndex]);
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
            Vector2 v1 = projectionPoints[a];
            Vector2 v2 = projectionPoints[b];
            Vector2 v3 = projectionPoints[c]; 
            DrawLine(v1.Vector2i(), v2.Vector2i(), 0xFF333333);
            DrawLine(v2.Vector2i(), v3.Vector2i(), 0xFF333333);
            DrawLine(v1.Vector2i(), v3.Vector2i(), 0xFF333333);
        }
        //점 그리기
        for (Vector2& point:projectionPoints)
        {
            Vector2& p = point;
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
void Renderer::DrawTriangle(Vector2 a, Vector2 b, Vector2 c, uint32_t color)
{
    // AABB 범위 계산
    float minX = floor(min({ a.x, b.x, c.x }));
    float maxX = ceil (max({ a.x, b.x, c.x }));
    float minY = floor(min({ a.y, b.y, c.y }));
    float maxY = ceil (max({ a.y, b.y, c.y }));
    

    for (int y = (int)minY; y <= (int)maxY; ++y) {
        for (int x = (int)minX; x <= (int)maxX; ++x) {

            Vector2 p = { (float)x + 0.5f, (float)y + 0.5f }; // 픽셀 중심 좌표

            // Barycentric 계산
            Vector2 v0 = b - a;
            Vector2 v1 = c - a;
            Vector2 v2 = p - a;

            float d00 = v0.Dot(v0);
            float d01 = v0.Dot(v1);
            float d11 = v1.Dot(v1);
            float d20 = v2.Dot(v0);
            float d21 = v2.Dot(v1);

            float denom = d00 * d11 - d01 * d01;

            float v = (d11 * d20 - d01 * d21) / denom;
            float w = (d00 * d21 - d01 * d20) / denom;
            float u = 1.0f - v - w;

            if (IsInsideTriangle(u, v, w)) {
                int index = y * width + x;
                colorBuffer[index] = color;
            }
        }
    }
}

bool Renderer::IsInsideTriangle(float u, float v, float w)
{
    return (u >= 0.0f) && (v >= 0.0f) && (w >= 0.0f);
}

int main(int arg, char** argv)
{
    Renderer renderer = Renderer();
    return 0;
}