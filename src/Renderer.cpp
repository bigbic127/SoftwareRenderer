#include "Renderer.hpp"
#include "Mesh.hpp"
#include "Transform.hpp"
#include "Camera.hpp"

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
}

void Renderer::Update()
{
    //FPS 프레임 딜레이
    int timeToWait = frameSecond - (SDL_GetTicks() - previousFrameSecond);
    if (timeToWait > 0 && timeToWait <= frameSecond)
        SDL_Delay(timeToWait);
    previousFrameSecond = SDL_GetTicks();


    Camera camera;
    camera.SetLookAt(Vector3(0.f,0.f, 5.f), Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f));
    camera.SetPerspective(60.f, width/height, 100.f, 0.1f);

    Transform transform;
    Vector3 rot = transform.GetRotation();
    rot.z += 0.1f * previousFrameSecond;
    transform.SetScale(Vector3(1,1,1));
    transform.SetRotation(Vector3(rot));
    Matrix4x4 meshPoint = transform.GetMatrix();
    Matrix4x4 camPoint = camera.GetMatrix();

    Mesh mesh = Mesh();
    vector<Triangle> triangle = mesh.GetIndices();
    vector<Vector3> vertices = mesh.GetVertices();
    projectionPoints.clear();

    for(Vector3& vertice:vertices)
    {
        Vector3 p = meshPoint * vertice;

        p.z -= 2;
        
        float screenX = ((100.f * p.x)/p.z)  + (width/2);
        float screenY = ((100.f * p.y)/p.z)  + (height/2);

        //float screenX = (p.x * 0.5f + 0.5f) * width;
        //float screenY = (p.y * 0.5f + 0.5f) * height;

        Vector2 vertex = Vector2(screenX, screenY);
        projectionPoints.push_back(vertex);
    }
}

void Renderer::Render()
{
    DrawClear();
    DrawGrid();
    //DrawPoint(10,10,4,4,0xFFFF0000);

    for (Vector2& point:projectionPoints)
    {
        Vector2& p = point;
        DrawPoint(p.x, p.y, 4,4,0xFFFF0000);
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

int main(int arg, char** argv)
{
    Renderer renderer = Renderer();
    return 0;
}