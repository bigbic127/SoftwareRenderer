#include "Mesh.hpp"
#include <vector>

Mesh::Mesh()
{
    // 박스 생성
    verticeNum = 8;
    vertices = new Vector3D[8]{
        {-1, 1, -1},
        {1, 1, -1},
        {1, -1, -1},
        {-1, -1, -1},
        {1, 1, 1},
        {1, -1, 1},
        {-1, 1, 1},
        {-1, -1, 1}
    };
    indicesNum = 12;
    indices = new Triangle[12]{
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
        {2, 1, 6},
        {2, 6, 7}
    };
    projectPoints = new Vector2D[verticeNum];
}