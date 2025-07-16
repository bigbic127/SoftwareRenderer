#include "RenderEngine.hpp"
#include "cube.hpp"
#include "Mesh.hpp"
#include "File.hpp"

#include <math.h>

RenderEngine::RenderEngine(int w, int h):RenderWindowSDL(w,h)
{
    Ready();
    LoopEvent();
}

void RenderEngine::LoopEvent()
{
    while(bIsLooping)
    {
        Update();
        Render();
    }
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void RenderEngine::Ready()
{
    if (Initalization())
        bIsLooping = true;

    //Ojb import
    vector<Vector3D>& tVertices = mesh.GetVertices();
    vector<Triangle>& tIndices = mesh.GetIndices();
    vector<Vector2D>& tProjectPostions = mesh.GetProjectPoints();

    tVertices.clear();
    tIndices.clear();
    LoadObjFile("../obj/teapot.obj", tVertices, tIndices);
    tProjectPostions.resize(tVertices.size());

    //obj 초기 회전 변경
    for (int i = 0 ; i < tVertices.size() ; i++)
    {
        Vector3D point = tVertices[i];

        // 큐브 회전
        Vector3D rot = mesh.GetRotation();
        rot.x = 180;
        mesh.SetRotation(rot);
        float angle = rot.x * (3.14 / 180);
        point = mesh.AddRotation_X(point, angle);
        tVertices[i] = point;
    }


}

void RenderEngine::Update()
{
    //FPS 에 맞춰 시간 딜레이
    int time_to_wait = frameSecond - (SDL_GetTicks() - PreviousFrameSecond);
    if (time_to_wait > 0 && time_to_wait <= frameSecond) {
        SDL_Delay(time_to_wait);
    }
    PreviousFrameSecond = SDL_GetTicks();


    //키도 W,S,A,D 로 카메라 위치 변경
    SetCamera(&camera);
    ProcessInput();

    // 메쉬(큐브) 포인트좌표를 2D 좌표로 변환
    int pointsNum = mesh.GetVertices().size();
    vector<Vector2D>& projectPoints = mesh.GetProjectPoints();
    Vector3D camPos = camera.GetPosition();
    vector<Vector3D>& vertice = mesh.GetVertices();

    for (int i = 0 ; i < pointsNum ; i++)
    {
        Vector3D point = vertice[i];

        // 큐브 회전
        Vector3D rot = mesh.GetRotation();
        rot.y = 0.5f;
        mesh.SetRotation(rot);
        float angle = rot.y * (3.14 / 180);
        point = mesh.AddRotation_Y(point, angle);

        vertice[i] = point;

        // 큐브 포지션 값에 카메라 위치 값 추가
        point.x -= camPos.x;
        point.y -= camPos.y; 
        point.z -= camPos.z - 2;

        Vector2D projectPoint = camera.GetProject(point);
        projectPoints[i] = projectPoint;
    }


}

void RenderEngine::Render()
{
    //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    //SDL_RenderClear(renderer);

    // 화면에 픽셀 그리기
    DrawClearColorBuffer(0xFF000000);
    DrawGrid(0xFF333333);
    //DrawRect(30, 30, 5, 5, 0XFFFFFFFF);

    //메쉬 큐브 그리기
    int pointsNum = mesh.GetVertices().size();
    vector<Vector2D> projectPoints = mesh.GetProjectPoints();
    
    for (int i = 0 ; i < pointsNum ; i++)
    {
        Vector2D projectPoint = projectPoints[i];
        //DrawRect(projectPoint.x, projectPoint.y, 5, 5, 0xFFFF0000);
    }

    const vector<Vector3D>& vertices = mesh.GetVertices();
    const vector<Triangle>& triangle = mesh.GetIndices();
    int indiceNum = triangle.size();
    for (int i = 0 ; i < indiceNum; i++)
    {
        int x = triangle[i].x;
        int y = triangle[i].y;
        int z = triangle[i].z;

        // backface culling 구현
        Vector3D v1 = vertices[x];
        Vector3D v2 = vertices[y];
        Vector3D v3 = vertices[z];

        Vector3D sV1 = SubVector(v2,v1);
        Vector3D sV2 = SubVector(v3,v1);
        Vector3D camV = SubVector(v1,camera.GetPosition());
        Normalize(sV1);
        Normalize(sV2);
        Normalize(camV);
        Vector3D cV = GetCrossVector(sV2,sV1);
        Normalize(cV);
        float dot = GetDotVector(camV, cV);
        if (dot < 0)
            continue;

        Vector2D line01 = projectPoints[x];
        Vector2D line02 = projectPoints[y];
        Vector2D line03 = projectPoints[z];

        //면 색상
        uint32_t color = 0xFFAAAAAA;
        if (mesh.GetColors().size() >= indiceNum)
            color = mesh.GetColors()[i];
        
        // 면 그리기
        DrawFill(line01.x, line01.y, line02.x, line02.y, line03.x, line03.y, color);// 0xFFAAAAAA);
        // 삼각형 라인 그리기
        DrawLine(line01.x, line01.y, line02.x, line02.y, 0xFFFFFFFF);
        DrawLine(line02.x, line02.y, line03.x, line03.y, 0xFFFFFFFF);
        DrawLine(line01.x, line01.y, line03.x, line03.y, 0xFFFFFFFF);
        // 점 그리기
        DrawRect(line01.x, line01.y, 2, 2, 0xFFFF0000);
        DrawRect(line02.x, line02.y, 2, 2, 0xFFFF0000);
        DrawRect(line03.x, line03.y, 2, 2, 0xFFFF0000);
    }

    // 컬러버퍼텍스쳐를 업데이트 후 렌더러에 적용
    SDL_UpdateTexture(colorBufferTexture, nullptr, colorBuffer, (int)(windowWidth *sizeof(uint32_t)));
    SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);

    SDL_RenderPresent(renderer);
}
