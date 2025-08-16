#include "Mesh.hpp"

void Mesh::Cube()
{
    Triangle tri;
    triangles.clear();
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
}

void Mesh::Sphere(int stacks, int slices, float radius)
{
    triangles.clear();
    vector<Vector3> vertices;
    vector<Vector3i> indices;
    vector<Vector3> normals;
    vector<Vector2> uvs;
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
            normals.push_back(Vector3(x, y, z).Normalized());
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
    for (const Vector3i& i : indices)
    {
        Triangle tri;
        Vector3 v1 = vertices[i.a];
        Vector3 v2 = vertices[i.b];
        Vector3 v3 = vertices[i.c];
        Vector3 n1 = normals[i.a];
        Vector3 n2 = normals[i.b];
        Vector3 n3 = normals[i.c];
        Vector2 uv1 = uvs[i.a];
        Vector2 uv2 = uvs[i.b];
        Vector2 uv3 = uvs[i.c];

        tri.vertices[0].pos = v1;
        tri.vertices[1].pos = v2;
        tri.vertices[2].pos = v3;
        tri.vertices[0].nor = n1;
        tri.vertices[1].nor = n2;
        tri.vertices[2].nor = n3;
        tri.vertices[0].uv = uv1;
        tri.vertices[1].uv = uv2;
        tri.vertices[2].uv = uv3;
        triangles.push_back(tri);
    }
}
