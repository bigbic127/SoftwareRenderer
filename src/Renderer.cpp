#include "Renderer.hpp"
#include "Mesh.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "File.hpp"
#include "FileDialog.hpp"
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
    Mesh mesh;
    meshes.clear();
    meshes.push_back(mesh);
    //카메라 파마리터 변경
    camera.SetLookAt(Vector3(0.f,0.5f, 4.f), Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f));
    camera.SetPerspective(70.f, float(width)/height, 0.1f, 100.f);
}

void Renderer::Update()
{
    //FPS 프레임 딜레이
    int timeToWait = frameSecond - (SDL_GetTicks() - previousFrameSecond);
    if (timeToWait > 0 && timeToWait <= frameSecond)
        SDL_Delay(timeToWait);
    previousFrameSecond = SDL_GetTicks();
    //Mesh Transform 처리
    if (bIsSpace)
    {
        for (Mesh&mesh : meshes)
        {
            Transform& transform = mesh.GetTransform();
            Vector3 rot = transform.GetRotation();
            rot.y += 0.5f * frameSecond;
            transform.SetRotation(rot);
        }
    }
}

void Renderer::Render()
{
    DrawClear();
    DrawGrid();
    //DrawPoint(10,10,4,4,0xFFFF0000);
    //메쉬 그리기
    for (Mesh&mesh : meshes)
    {
        vector<Vector3> vertices = mesh.GetVertices();
        vector<Triangle> indices = mesh.GetIndices();
        vector<Vector2> uvs = mesh.GetUVs();
        vector<Triangle> uvIndices = mesh.GetUVIndices();
        vector<Vector3> worldVertices = mesh.GetWorldVertices();
        vector<uint32_t> colors = mesh.GetColors();
        Vector3 cameraPosition = camera.GetPosition();
        vector<Vector3>& projectionPoints = mesh.GetPojectionPoint();
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
            for (int i = 0; i < indices.size(); i++)
            {
                Triangle& tri = indices[i];
                int a = tri.a;
                int b = tri.b;
                int c = tri.c;
                Vector3 v1 = projectionPoints[a];
                Vector3 v2 = projectionPoints[b];
                Vector3 v3 = projectionPoints[c];
                //UV z값 보간 (view행렬의 w값)
                Triangle& triuv = uvIndices[i];
                int _ua = triuv.a;
                int _ub = triuv.b;
                int _uc = triuv.c;
                Vector2 uv1 = uvs[_ua];
                Vector2 uv2 = uvs[_ub];
                Vector2 uv3 = uvs[_uc];
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
                DrawTriangle(v1, v2, v3, color, bIsTextureMode, uv1, uv2, uv3, mesh);
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
            for (Triangle& tri: indices)
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
        {
            SetRenderMode(RenderMode::Shader);
            bIsTextureMode = false;
        }
        else if (event.key.keysym.sym == SDLK_4)
        {
            SetRenderMode(RenderMode::Solid);
            bIsTextureMode = true;
        }
        else if (event.key.keysym.sym == SDLK_o)
            OpenObjFile();
        else if (event.key.keysym.sym == SDLK_SPACE)
            bIsSpace = !bIsSpace;
        else if (event.key.keysym.sym == SDLK_r)
            camera.SetLookAt(Vector3(0.f,0.5f, 4.f), Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f));
        InputTransform(event);
        break;
    case SDL_KEYUP:
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            bIsClicked = true;
            screenPos.x = event.button.x;
            screenPos.y = event.button.y;
            screenOriPos = camera.GetPosition();
        }
        else if (event.button.button == SDL_BUTTON_MIDDLE)
        {
            bIsMClicked = true;
            screenPos.x = event.button.x;
            screenPos.y = event.button.y;
            screenOriPos = camera.GetPosition();
            targetPos = camera.GetTarget();
        }
        break;
    case SDL_MOUSEBUTTONUP:
        if (event.button.button == SDL_BUTTON_LEFT)
            bIsClicked = false;
        else if (event.button.button == SDL_BUTTON_MIDDLE)
            bIsMClicked = false;
        break;
    case SDL_MOUSEWHEEL:
    {
        Vector3 cameraPosition = camera.GetPosition();
        Vector3 cameraTarget = camera.GetTarget();
        Vector3 cameraNormal;
        cameraNormal = (cameraTarget - cameraPosition).Normalized() * (event.wheel.y * -0.25f);
        cameraPosition = cameraPosition - cameraNormal;
        camera.SetLookAt(cameraPosition, cameraTarget, Vector3(0.f,1.f,0.f));
    }
        break;
    case SDL_MOUSEMOTION:
        if (bIsClicked)
        {
            Vector3 cameraTarget = camera.GetTarget();
            Transform transform;
            Vector2 motion, vec;
            motion.x = event.motion.x;
            motion.y = event.motion.y;
            vec = (motion - screenPos)*-0.2f;
            Vector3 up = Vector3(0.f,1.f,0.f);
            Vector3 cross =  (cameraTarget - camera.GetPosition()).Normalized().Cross(up);
            float pitchAngle = DEG2RAD(vec.y);
            float yawAngle = DEG2RAD(vec.x);
            Vector3 rotatedOffset = transform.GetRotateAroundAxis(screenOriPos, cross, pitchAngle);
            rotatedOffset = transform.GetRotateAroundAxis(rotatedOffset, up, yawAngle);
            Vector3 cameraPosition = rotatedOffset;
            camera.SetLookAt(cameraPosition, cameraTarget, Vector3(0.f,1.f,0.f));
        }
        else if (bIsMClicked)
        {
            Vector2 motion, vec;
            motion.x = event.motion.x;
            motion.y = event.motion.y;
            vec = (motion - screenPos) * 0.001f;
            Vector3 up = Vector3(0.f,1.f,0.f);
            Vector3 cross =  (camera.GetTarget() - camera.GetPosition()).Normalized().Cross(up);
            Vector3 cameraPosition = cross * -vec.x  + up * vec.y + screenOriPos;
            Vector3 cameraTarget = cross * -vec.x  + up * vec.y + targetPos;
            camera.SetLookAt(cameraPosition, cameraTarget, up);

        }
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
void Renderer::DrawTriangle(Vector3 a, Vector3 b, Vector3 c, uint32_t color, bool bIsTex, Vector2 uv0, Vector2 uv1, Vector2 uv2, Mesh& mesh)
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
                // Barycentric interpolation (Z값 보간)
                float z = a.z*w0 + b.z*w1 + c.z*w2;
                // 보간된 UV
                float u = uv0.x*w0 + uv1.x*w1 + uv2.x*w2;
                float v = uv0.y*w0 + uv1.y*w1 + uv2.y*w2;
                int index = y * width + x;
                if (z < zBuffer[index])
                {
                    zBuffer[index] = z;
                    //DrawPoint(x, y, 5, 5, color);
                    if(bIsTex && mesh.GetTexture().size() > 0) color = DrawTexture(u, v, mesh);
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

uint32_t Renderer::DrawTexture(float u, float v, Mesh& mesh)
{
    vector<uint32_t>& texture = mesh.GetTexture();
    int& width = mesh.GetUVsWidth();
    int& height = mesh.GetUVsHeight();
    v = 1.0f - v; // ← Y축 뒤집기
    float texX = u * width - 0.5f;
    float texY = v * height - 0.5f;
    int x = SDL_clamp(int(texX + 0.5f), 0, (int)width - 1);
    int y = SDL_clamp(int(texY + 0.5f), 0, (int)height - 1);
    uint32_t pixel = texture[y * width + x];
    return pixel;
}

void Renderer::OpenObjFile()
{

    std::vector<std::filesystem::path> paths = FileDialog::ShowFileDialog();
    if (paths.size() <= 0)
        return;
    meshes.clear();
    for (auto& path : paths)
    {
        std::filesystem::path objPath = path;
        std::filesystem::path pngPath = path;
        if (objPath.extension() == ".glb")
        {
            vector<Mesh> rMeshes;
            rMeshes = LoadGLTF(path.string());
            for (Mesh mesh : rMeshes)
                meshes.push_back(mesh);
        }
        else if (objPath.extension() == ".obj")
        {
            pngPath.replace_extension(".png");
            Mesh mesh = Mesh();
            LoadObjFile(objPath.string(), mesh);
            LoadPngFile(pngPath.string(), mesh);
            meshes.push_back(mesh);
        }
    }
    //메쉬 스케일링
    Vector3 minBound = { FLT_MAX, FLT_MAX, FLT_MAX };
    Vector3 maxBound = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
    for (Mesh& mesh : meshes)
    {
        for (const auto& v : mesh.GetVertices()) {
            minBound.x = std::min(minBound.x, v.x);
            minBound.y = std::min(minBound.y, v.y);
            minBound.z = std::min(minBound.z, v.z);
            maxBound.x = std::max(maxBound.x, v.x);
            maxBound.y = std::max(maxBound.y, v.y);
            maxBound.z = std::max(maxBound.z, v.z);
        }
    }
    Vector3 size = minBound - maxBound;
    Vector3 center = (minBound + maxBound) * 0.5f;
    float max_dim = std::max({size.x, size.y, size.z});
    float value = 1.0f;
    float scale_factor = value / max_dim;
    float scale_factor2 = -value * scale_factor;
    for (Mesh& mesh : meshes)
    {
        mesh.GetTransform().SetPosition(center * scale_factor);
        mesh.GetTransform().SetScale(Vector3(scale_factor2,scale_factor2,scale_factor2));
    }
    camera.SetLookAt(Vector3(0.f,0.5f, 4.f), Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f));
    camera.SetPerspective(70.f, float(width)/height, 0.1f, 100.f);
}

void Renderer::InputTransform(SDL_Event& event)
{
    switch (event.type)
    {
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_UP)
            {
                for (Mesh&mesh : meshes)
                {
                    Transform& transform = mesh.GetTransform();
                    Vector3 rot = transform.GetRotation();
                    rot.x += 0.5f * frameSecond;
                    transform.SetRotation(rot);
                }
            }
            else if (event.key.keysym.sym == SDLK_DOWN)
            {
                for (Mesh&mesh : meshes)
                {
                    Transform& transform = mesh.GetTransform();
                    Vector3 rot = transform.GetRotation();
                    rot.x -= 0.5f * frameSecond;
                    transform.SetRotation(rot);
                }
            }
            else if (event.key.keysym.sym == SDLK_RIGHT)
            {
                for (Mesh&mesh : meshes)
                {
                    Transform& transform = mesh.GetTransform();
                    Vector3 rot = transform.GetRotation();
                    rot.y += 0.5f * frameSecond;
                    transform.SetRotation(rot);
                }
            }
            else if (event.key.keysym.sym == SDLK_LEFT)
            {
                for (Mesh&mesh : meshes)
                {
                    Transform& transform = mesh.GetTransform();
                    Vector3 rot = transform.GetRotation();
                    rot.y -= 0.5f * frameSecond;
                    transform.SetRotation(rot);
                }
            }
    }
}

int main(int arg, char** argv)
{
    Renderer renderer = Renderer();
    return 0;
}