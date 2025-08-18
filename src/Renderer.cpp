#include "Renderer.hpp"
#include "Mesh.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "File.hpp"
#include "FileDialog.hpp"
#include "Clipping.hpp"
#include <cmath>

using namespace std;

Renderer::Renderer()
{   
    Init();
    Ready();
    Loop();
}
void Renderer::Init()
{
}

void Renderer::Loop()
{
    while(bIsLooping)
    {
        Input();
        DrawGUI();
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
    //mesh.GetTransform().SetPosition(Vector3(2,0,0));
    meshes.clear();
    meshes.push_back(mesh);
    //카메라 파마리터 변경
    camera.SetLookAt(Vector3(0.f, 1.5f, 4.f), Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f));
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
            rot.y += 0.2f * frameSecond;
            transform.SetRotation(rot);
        }
    }
}

void Renderer::Render()
{
    DrawClear();
    DrawGrid();
    //DrawPoint(10,10,4,4,0xFFFF0000);
    for (Mesh&mesh : meshes)
    {
        vector<Triangle>& triangles = mesh.GetTriangles();
        triangles.clear();
        Vector3 cameraPosition = camera.GetPosition();
        vector<Vector4> planes = camera.GetFrustumPlanes();
        vector<Vertex>& vertex = mesh.GetVertex();
        vector<Vector3i>& indices = mesh.GetIndices();
        for(Vertex& vtx: vertex)
        {
            Vector4 v = vtx.pos;
            //Bind Skin 작업
            if (scene.joints.size()>0)
            {
                std::array<int, 4> jointIndices = {vtx.jointIndices.x, vtx.jointIndices.y, vtx.jointIndices.z, vtx.jointIndices.w};
                std::array<float, 4> weightValues = {vtx.weights.x, vtx.weights.y, vtx.weights.z, vtx.weights.w};
                Vector4 skinned;
                for(size_t i=0; i < 4; i++)
                {
                    int j = jointIndices[i];
                    float w = weightValues[i];
                    int _j = scene.joints[0].children[j];
                    if (w > 0.0f)
                        skinned =  skinned + ((scene.nodes[_j].transform.GetMatrix() * v) * w);
                }
                v = Vector4(skinned.x, skinned.y, skinned.z, 1.0f);
            }
            Vector4 model = mesh.GetTransform().GetMatrix() * v; //월드행렬 변환
            Vector4 view =  camera.GetViewMatrix() * model; //뷰행렬 변환
            Vector4 projection = camera.GetProjectionMatrix() * view; //프로젝션행렬 변환
            vtx.proj_m = Vector3(model.x, model.y, model.z);
            vtx.proj_clip = projection;
        }
        for(Vector3i& index : indices)
        {
            //backface culling 계산
            Vector3 v1 = vertex[index.a].proj_m;
            Vector3 v2 = vertex[index.b].proj_m;
            Vector3 v3 = vertex[index.c].proj_m;
            Vector3 triCenter = (v1 + v2 + v3) / 3.0f;
            Vector3 cameraDir = (triCenter - cameraPosition).Normalized();
            Vector3 normal = ((v2-v1).Cross(v3-v1)).Normalized();
            if (normal.Dot(cameraDir) >= 0 && renderMode != RenderMode::Wireframe)
                continue;
            // 클리핑 계산
            Triangle tri = {vertex[index.a], vertex[index.b], vertex[index.c]};
            vector<Vector4> planes = camera.GetFrustumPlanes();
            ClipTriangleAgainstFrustum(tri, planes, triangles);
        }
        for (Triangle& tri : triangles)
        {
            // 클리핑된 삼각형 NDC 좌표 변환
            for (Vertex& vertice : tri.vertices)
            {
                Vector4 projection = vertice.proj_clip;
                Vector3 p = Vector3(projection.x/projection.w , projection.y/projection.w, projection.z/projection.w);//NDC 좌표계로 변환
                float screenX = (p.x * 0.5f + 0.5f) * width;
                float screenY = (1.0f - (p.y * 0.5f + 0.5f)) * height;
                float zdepth = p.z * 0.5f + 0.5f;
                vertice.proj_p = Vector3(screenX, screenY, zdepth);
            }
            if (renderMode == RenderMode::Solid || renderMode == RenderMode::Shader)
            {
                //광원 및 색상
                Vector3 v1 = tri.vertices[0].proj_m;
                Vector3 v2 = tri.vertices[1].proj_m;
                Vector3 v3 = tri.vertices[2].proj_m;
                Vector3 triCenter = (v1 + v2 + v3) / 3.0f;
                Vector3 cameraDir = (triCenter - cameraPosition).Normalized();
                Vector3 normal = ((v2-v1).Cross(v3-v1)).Normalized();
                float brightness = max(0.0f, normal.Dot(cameraDir * -1));
                brightness *= 1.0f;
                int gray = static_cast<int>(brightness * 255.0f);
                uint32_t color = 0xFF000000 | (gray << 16) | (gray << 8) | gray;// ARGB  각uint8
                mesh.GetColor() = color;
            }
            else
                mesh.GetColor() = 0xFF555555;
            if (renderMode == RenderMode::Solid || renderMode == RenderMode::FloatData || renderMode == RenderMode::Shader)
                {
                    vector<Vector3> projectionPoints = {
                        tri.vertices[0].proj_p,
                        tri.vertices[1].proj_p,
                        tri.vertices[2].proj_p};
                    vector<Vector2> uvs = {
                        tri.vertices[0].uv,
                        tri.vertices[1].uv,
                        tri.vertices[2].uv};
                    DrawTriangle(projectionPoints, uvs, mesh);
                }
            if (renderMode == RenderMode::Wireframe || renderMode == RenderMode::FloatData)
            {
                Vector3 v1 = tri.vertices[0].proj_p;
                Vector3 v2 = tri.vertices[1].proj_p;
                Vector3 v3 = tri.vertices[2].proj_p;
                DrawLine(v1.toVector2i(), v2.toVector2i(), 0xFF00FFFF);
                DrawLine(v2.toVector2i(), v3.toVector2i(), 0xFF00FFFF);
                DrawLine(v1.toVector2i(), v3.toVector2i(), 0xFF00FFFF);
                DrawPoint(v1.x, v1.y, 4,4,0xFFFFFFFF);
                DrawPoint(v2.x, v2.y, 4,4,0xFFFFFFFF);
                DrawPoint(v3.x, v3.y, 4,4,0xFFFFFFFF);
            }
        }
    }
    //텍스쳐에 컬러버퍼에 들어있는 데이터 적용
    SDL_UpdateTexture(colorBufferTexture, nullptr, colorBuffer.get(), (int)(width * sizeof(uint32_t)));
    SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);//ImGUI 그리기
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
            SetRenderMode(RenderMode::Solid);
        else if (event.key.keysym.sym == SDLK_4)
            SetRenderMode(RenderMode::Shader);
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
    {
        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureMouse)
            break;
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
    }
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
void Renderer::DrawTriangle(vector<Vector3>& p, vector<Vector2>& uv, Mesh& mesh)
{
    //2D좌표로 변환
    Vector3 p1 = p[0];
    Vector3 p2 = p[1];
    Vector3 p3 = p[2];
    //UV
    Vector2 uv1 = uv[0];
    Vector2 uv2 = uv[1];
    Vector2 uv3 = uv[2];
    //AABB 범위 계산
    float minX = max(0.0f, floor(min({ p1.x, p2.x, p3.x })));
    float maxX = min((float)width - 1, ceil(max({ p1.x, p2.x, p3.x })));
    float minY = max(0.0f, floor(min({ p1.y, p2.y, p3.y })));
    float maxY = min((float)height - 1, ceil(max({ p1.y, p2.y, p3.y })));
    float area = EdgeFunction(p1.toVector2(), p2.toVector2(), p3.toVector2());
    if (area == 0.0f)
        return; //삼각형이 아니면 그리지 않음
    for (int y = (int)minY; y <= (int)maxY; ++y) {
        for (int x = (int)minX; x <= (int)maxX; ++x) {
            Vector2 p = {x + 0.5f, y + 0.5f}; //픽셀 중심 좌표
            float w1 = EdgeFunction(p2.toVector2(), p3.toVector2(), p);
            float w2 = EdgeFunction(p3.toVector2(), p1.toVector2(), p);
            float w3 = EdgeFunction(p1.toVector2(), p2.toVector2(), p);
            //픽셀이 삼각형 내부에 있는지 확인 (모든 barycentric 계수가 양수이면 내부)
            if (w1 >= 0 && w2 >= 0 && w3 >= 0)
            {
                w1 /= area;
                w2 /= area;
                w3 /= area;
                // Barycentric interpolation (Z값 보간)
                float z = p1.z*w1 + p2.z*w2 + p3.z*w3;
                // 보간된 UV
                float u = uv1.x*w1 + uv2.x*w2 + uv3.x*w3;
                float v = uv1.y*w1 + uv2.y*w2 + uv3.y*w3;
                int index = y * width + x;
                if (z < zBuffer[index])
                {
                    zBuffer[index] = z;
                    //DrawPoint(x, y, 5, 5, color);
                    if(mesh.GetTexture().size() > 0 && renderMode == RenderMode::Shader) mesh.GetColor() = DrawTexture(u, v, mesh);
                    colorBuffer[index] = mesh.GetColor();
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
            scene = LoadGLTF(path.string());
            for (Mesh mesh : scene.meshes)
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
        for (const Vertex& v : mesh.GetVertex())
        {
            minBound.x = std::min(minBound.x, v.pos.x);
            minBound.y = std::min(minBound.y, v.pos.y);
            minBound.z = std::min(minBound.z, v.pos.z);
            maxBound.x = std::max(maxBound.x, v.pos.x);
            maxBound.y = std::max(maxBound.y, v.pos.y);
            maxBound.z = std::max(maxBound.z, v.pos.z);
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

void Renderer::IRenderMode(int mode)
{
    switch (mode)
    {
    case 0:
        SetRenderMode(RenderMode::Wireframe);
        break;
    case 1:
        SetRenderMode(RenderMode::FloatData);
        break;
    case 2:
        SetRenderMode(RenderMode::Solid);
        break;
    case 3:
        SetRenderMode(RenderMode::Shader);
        break;
    default:
        break;
    }
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