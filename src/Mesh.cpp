#include "Mesh.hpp"

void Mesh::Cube()
{
    vertices = {
        {-1, 1, -1},
        {1, 1, -1},
        {1, -1, -1},
        {-1, -1, -1},
        {1, 1, 1},
        {1, -1, 1},
        {-1, 1, 1},
        {-1, -1, 1}
    };
    indices = {
        //삼각형 그리기 시계방향으로 vertices 인텍스 번호
        {3, 0, 1},
        {3, 1, 2},
        {2, 1, 4},
        {2, 4, 5},
        {7, 6, 0},
        {7, 0, 3},
        {0, 6, 4},
        {0, 4, 1},
        {7, 3, 2},
        {7, 2, 5},
        {5, 4, 6},
        {5, 6, 7}
    };
    colors = {
        0xFFFF0000,
        0xFFFF0000,
        0xFF00FF00,
        0xFF00FF00,
        0xFF0000FF,
        0xFF0000FF,
        0xFFFFFF00,
        0xFFFFFF00,
        0xFFFF00FF,
        0xFFFF00FF,
        0xFF00FFFF,
        0xFF00FFFF
    };
}