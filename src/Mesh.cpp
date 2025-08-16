#include "Mesh.hpp"

void Mesh::Cube()
{
    triangles.clear();
    vertex.clear();
    vertices.clear();
    indices.clear();
    uvs.clear();
    vertices =
    {
        {-1,  1, -1}, // 0 - 왼쪽 위 앞
        { 1,  1, -1}, // 1 - 오른쪽 위 앞
        { 1, -1, -1}, // 2 - 오른쪽 아래 앞
        {-1, -1, -1}, // 3 - 왼쪽 아래 앞
        {-1,  1,  1}, // 4 - 왼쪽 위 뒤
        { 1,  1,  1}, // 5 - 오른쪽 위 뒤
        { 1, -1,  1}, // 6 - 오른쪽 아래 뒤
        {-1, -1,  1}  // 7 - 왼쪽 아래 뒤
    };
    indices =
    {
        // 앞면
        {0, 2, 3},
        {0, 1, 2},
        // 뒷면
        {4, 6, 5},
        {4, 7, 6},
        // 왼쪽면
        {0, 7, 4},
        {0, 3, 7},
        // 오른쪽면
        {1, 6, 2},
        {1, 5, 6},
        // 윗면
        {0, 5, 1},
        {0, 4, 5},
        // 아랫면
        {3, 6, 7},
        {3, 2, 6}
    };
    uvs =
    {
        {0.0f, 1.0f}, // 0 - 왼쪽 위 앞
        {1.0f, 1.0f}, // 1 - 오른쪽 위 앞
        {1.0f, 0.0f}, // 2 - 오른쪽 아래 앞
        {0.0f, 0.0f}, // 3 - 왼쪽 아래 앞
        {0.0f, 1.0f}, // 4 - 왼쪽 위 뒤
        {1.0f, 1.0f}, // 5 - 오른쪽 위 뒤
        {1.0f, 0.0f}, // 6 - 오른쪽 아래 뒤
        {0.0f, 0.0f}  // 7 - 왼쪽 아래 뒤
    };
    // Triangle 구조
    Triangle tri;
    // 앞
    tri.vertices[0].pos = Vector4(-1, 1, -1, 1.0f);
    tri.vertices[1].pos = Vector4(1, -1, -1, 1.0f);
    tri.vertices[2].pos = Vector4(-1, -1, -1, 1.0f);
    tri.vertices[0].uv = Vector2(0.0f, 1.0f);
    tri.vertices[1].uv = Vector2(1.0f, 0.0f);
    tri.vertices[2].uv = Vector2(0.0f, 0.0f);
    triangles.push_back(tri);
    tri.vertices[0].pos = Vector4(-1, 1, -1, 1.0f);
    tri.vertices[1].pos = Vector4(1, 1, -1, 1.0f);
    tri.vertices[2].pos = Vector4(1, -1, -1, 1.0f);
    tri.vertices[0].uv = Vector2(0.0f, 1.0f);
    tri.vertices[1].uv = Vector2(1.0f, 1.0f);
    tri.vertices[2].uv = Vector2(1.0f, 0.0f);
    triangles.push_back(tri);
    // 뒤
    tri.vertices[0].pos = Vector4(-1, -1, 1, 1.0f);
    tri.vertices[1].pos = Vector4(1, 1, 1, 1.0f);
    tri.vertices[2].pos = Vector4(-1, 1, 1, 1.0f);
    tri.vertices[0].uv = Vector2(0.0f, 0.0f);
    tri.vertices[1].uv = Vector2(1.0f, 1.0f);
    tri.vertices[2].uv = Vector2(0.0f, 1.0f);
    triangles.push_back(tri);
    tri.vertices[0].pos = Vector4(-1, -1, 1, 1.0f);
    tri.vertices[1].pos = Vector4(1, -1, 1, 1.0f);
    tri.vertices[2].pos = Vector4(1, 1, 1, 1.0f);
    tri.vertices[0].uv = Vector2(0.0f, 0.0f);
    tri.vertices[1].uv = Vector2(1.0f, 0.0f);
    tri.vertices[2].uv = Vector2(1.0f, 1.0f);
    triangles.push_back(tri);
    // 오른쪽
    tri.vertices[0].pos = Vector4(1, 1, -1, 1.0f);
    tri.vertices[1].pos = Vector4(1, -1, 1, 1.0f);
    tri.vertices[2].pos = Vector4(1, -1, -1, 1.0f);
    tri.vertices[0].uv = Vector2(0.0f, 1.0f);
    tri.vertices[1].uv = Vector2(1.0f, 0.0f);
    tri.vertices[2].uv = Vector2(0.0f, 0.0f);
    triangles.push_back(tri);
    tri.vertices[0].pos = Vector4(1, 1, -1, 1.0f);
    tri.vertices[1].pos = Vector4(1, 1, 1, 1.0f);
    tri.vertices[2].pos = Vector4(1, -1, 1, 1.0f);
    tri.vertices[0].uv = Vector2(0.0f, 1.0f);
    tri.vertices[1].uv = Vector2(1.0f, 1.0f);
    tri.vertices[2].uv = Vector2(1.0f, 0.0f);
    triangles.push_back(tri);
    // 왼쪽
    tri.vertices[0].pos = Vector4(-1, 1, 1, 1.0f);
    tri.vertices[1].pos = Vector4(-1, -1, -1, 1.0f);
    tri.vertices[2].pos = Vector4(-1, -1, 1, 1.0f);
    tri.vertices[0].uv = Vector2(0.0f, 1.0f);
    tri.vertices[1].uv = Vector2(1.0f, 0.0f);
    tri.vertices[2].uv = Vector2(0.0f, 0.0f);
    triangles.push_back(tri);
    tri.vertices[0].pos = Vector4(-1, 1, 1, 1.0f);
    tri.vertices[1].pos = Vector4(-1, 1, -1, 1.0f);
    tri.vertices[2].pos = Vector4(-1, -1, -1, 1.0f);
    tri.vertices[0].uv = Vector2(0.0f, 1.0f);
    tri.vertices[1].uv = Vector2(1.0f, 1.0f);
    tri.vertices[2].uv = Vector2(1.0f, 0.0f);
    triangles.push_back(tri);
    // 위
    tri.vertices[0].pos = Vector4(-1, 1, 1, 1.0f);
    tri.vertices[1].pos = Vector4(1, 1, 1, 1.0f);
    tri.vertices[2].pos = Vector4(-1, 1, -1, 1.0f);
    tri.vertices[0].uv = Vector2(0.0f, 1.0f);
    tri.vertices[1].uv = Vector2(1.0f, 1.0f);
    tri.vertices[2].uv = Vector2(0.0f, 0.0f);
    triangles.push_back(tri);
    tri.vertices[0].pos = Vector4(1, 1, 1, 1.0f);
    tri.vertices[1].pos = Vector4(1, 1, -1, 1.0f);
    tri.vertices[2].pos = Vector4(-1, 1, -1, 1.0f);
    tri.vertices[0].uv = Vector2(1.0f, 1.0f);
    tri.vertices[1].uv = Vector2(1.0f, 0.0f);
    tri.vertices[2].uv = Vector2(0.0f, 0.0f);
    triangles.push_back(tri);
    // 아래
    tri.vertices[0].pos = Vector4(-1, -1, -1, 1.0f);
    tri.vertices[1].pos = Vector4(1, -1, -1, 1.0f);
    tri.vertices[2].pos = Vector4(-1, -1, 1, 1.0f);
    tri.vertices[0].uv = Vector2(0.0f, 0.0f);
    tri.vertices[1].uv = Vector2(1.0f, 0.0f);
    tri.vertices[2].uv = Vector2(0.0f, 1.0f);
    triangles.push_back(tri);
    tri.vertices[0].pos = Vector4(1, -1, -1, 1.0f);
    tri.vertices[1].pos = Vector4(1, -1, 1, 1.0f);
    tri.vertices[2].pos = Vector4(-1, -1, 1, 1.0f);
    tri.vertices[0].uv = Vector2(1.0f, 0.0f);
    tri.vertices[1].uv = Vector2(1.0f, 1.0f);
    tri.vertices[2].uv = Vector2(0.0f, 1.0f);
    triangles.push_back(tri);
    //Vertex 구조
    for (size_t i = 0; i < vertices.size(); i++)
    {
        Vertex v;
        v.pos = Vector4(vertices[i]);
        v.uv = uvs[i];
        vertex.push_back(v);
    }
}

void Mesh::Sphere(int stacks, int slices, float radius)
{
    triangles.clear();
    vertex.clear();
    vertices.clear();
    indices.clear();
    uvs.clear();
    constexpr float PI = 3.14159265359f;
    for (int lat = 0; lat <= stacks; ++lat)
    {
        float theta = lat * PI / stacks;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);
        for (int lon = 0; lon <= slices; ++lon)
        {
            float phi = lon * 2.0f * PI / slices;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);
            float x = radius * sinTheta * cosPhi;
            float y = radius * cosTheta;
            float z = radius * sinTheta * sinPhi;
            vertices.push_back(Vector3(x, y, z));
            float u = static_cast<float>(lon) / slices;
            float v = static_cast<float>(lat) / stacks;
            uvs.push_back(Vector2(u, v));
        }
    }
    for (int lat = 0; lat < stacks; ++lat)
    {
        for (int lon = 0; lon < slices; ++lon)
        {
            int current = lat * (slices + 1) + lon;
            int next = current + slices + 1;
            indices.push_back(Vector3i(current, current + 1, next));
            indices.push_back(Vector3i(current + 1, next + 1, next));
        }
    }
    // Triangle 구조
    for (const Vector3i& i : indices)
    {
        Triangle tri;
        Vector3 v1 = vertices[i.a];
        Vector3 v2 = vertices[i.b];
        Vector3 v3 = vertices[i.c];
        Vector2 uv1 = uvs[i.a];
        Vector2 uv2 = uvs[i.b];
        Vector2 uv3 = uvs[i.c];

        tri.vertices[0].pos = v1;
        tri.vertices[1].pos = v2;
        tri.vertices[2].pos = v3;
        tri.vertices[0].uv = uv1;
        tri.vertices[1].uv = uv2;
        tri.vertices[2].uv = uv3;
        triangles.push_back(tri);
    }
    //Vertex 구조
    for (size_t i = 0; i < vertices.size(); i++)
    {
        Vertex v;
        v.pos = Vector4(vertices[i]);
        v.uv = uvs[i];
        vertex.push_back(v);
    }
}
