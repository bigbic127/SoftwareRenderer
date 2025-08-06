#include "Mesh.hpp"

void Mesh::Cube()
{
    vertices.clear();
    indices.clear();
    colors.clear();
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
    /*Z버퍼 테스트
    vertices = {
    { -1, -1, -1 },
    { 1, -1, -1 },
    { 1,  1, -1 },
    { -1,  1, -1 },
    { -1, -1,  1 },
    { 1, -1,  1 },
    { 1,  1,  1 },
    { -1,  1,  1 }
    };
    */
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

void Mesh::Sphere(int stacks, int slices, float radius)
{
    vertices.clear();
    indices.clear();
    colors.clear();
    for (int i = 0; i <= stacks; ++i)
    {
        float v = float(i) / stacks;
        float theta = v * 3.14159;
        for (int j = 0; j <= slices; ++j)
        {
            float u = float(j) / slices;
            float phi = u * 2 * 3.14159;
            float x = radius * sinf(theta) * cosf(phi);
            float y = radius * cosf(theta);
            float z = radius * sinf(theta) * sinf(phi);
            vertices.push_back(Vector3(x, y, z));
        }
    }
    for (int i = 0; i < stacks; ++i)
    {
        for (int j = 0; j < slices; ++j)
        {
            int first  = i * (slices + 1) + j;
            int second = first + slices + 1;
            indices.push_back(Triangle(first + 1, second, first));
            indices.push_back(Triangle(first + 1, second + 1, second));
        }
    }
}