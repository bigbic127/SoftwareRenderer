#include "RenderEngine.hpp"
#include "cube.hpp"

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

    // 3차원 큐브의 포인트좌표를 2D 좌표로 변환
    int pointsNum = cube.GetPointsNum();
    projectPoints = cube.GetProjectPoints();
    Vector3D camPos = camera.GetPosition();

    for (int i = 0 ; i < pointsNum ; i++)
    {
        Vector3D point = cube.GetPoints()[i];

        // 큐브 회전
        Vector3D rot = cube.GetRotation();
        rot.x += 0.001f ;
        cube.SetRotation(rot);
        float angle = rot.x * (3.14 / 180);
        point = cube.AddRotation_X(point, angle);
        point = cube.AddRotation_Y(point, angle);
        point = cube.AddRotation_Z(point, angle);

        // 큐브 포지션 값에 카메라 위치 값 추가
        point.x -= camPos.x;
        point.y -= camPos.y; 
        point.z -= camPos.z;

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

    //3D큐브 그리기
    int pointsNum = cube.GetPointsNum();
    projectPoints = cube.GetProjectPoints();
    
    for (int i = 0 ; i < pointsNum ; i++)
    {
        Vector2D projectPoint = projectPoints[i];
        DrawRect(projectPoint.x + (windowWidth/2), projectPoint.y + (windowHeight/2), 5, 5, 0xFFFFFFFF);
    }

    // 컬러버퍼텍스쳐를 업데이트 후 렌더러에 적용
    SDL_UpdateTexture(colorBufferTexture, nullptr, colorBuffer, (int)(windowWidth *sizeof(uint32_t)));
    SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);

    SDL_RenderPresent(renderer);
}
