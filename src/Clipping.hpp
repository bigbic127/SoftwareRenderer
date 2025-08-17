#pragma once
#include "Vector.hpp"
#include "Mesh.hpp"
#include <vector>

// 점과 평면 사이의 부호 있는 거리를 계산
static float GetSignedDistanceToPlane(const Vertex& v, const Vector4& plane)
{
    return plane.x * v.proj_clip.x + plane.y * v.proj_clip.y + plane.z * v.proj_clip.z + plane.w * v.proj_clip.w;
}

// 두 점(내부점, 외부점)을 잇는 선과 평면의 교차점을 계산
static Vertex IntersectPlane(Vertex& inside_v, Vertex& outside_v, const Vector4& plane)
{
    float d1 = GetSignedDistanceToPlane(inside_v, plane);
    float d2 = GetSignedDistanceToPlane(outside_v, plane);
    float t = d1 / (d1 - d2);// 교차점까지의 비율
    Vertex vertex;
    vertex.proj_clip = inside_v.proj_clip.Lerp(outside_v.proj_clip, t);
    vertex.proj_m = inside_v.proj_m.Lerp(outside_v.proj_m, t);
    vertex.uv = inside_v.uv.Lerp(outside_v.uv, t);
    return vertex;
}

void ClipTriangleAgainstFrustum(const Triangle& in_triangle, const std::vector<Vector4>& planes, std::vector<Triangle>& out_triangles)
{
    std::vector<Vertex> list_vertices;
    for (int i = 0; i < 3; ++i)
        list_vertices.push_back(in_triangle.vertices[i]);
    // 6개의 평면에 대해 순차적으로 클리핑
    for (const auto& plane : planes)
    {
        std::vector<Vertex> new_list_vertices;
        // 다각형의 각 변(edge)에 대해 클리핑
        for (size_t i = 0; i < list_vertices.size(); ++i)
        {
            Vertex& current_v =  list_vertices[i];
            Vertex& prev_v =  list_vertices[(i + list_vertices.size() - 1) % list_vertices.size()];
            float dist_current = GetSignedDistanceToPlane(current_v, plane);
            float dist_prev = GetSignedDistanceToPlane(prev_v, plane);
            // Case 1: 두 점 모두 안쪽 -> 현재 점 추가
            if (dist_current >= 0 && dist_prev >= 0)
                new_list_vertices.push_back(current_v);
            // Case 2: 이전 점은 안쪽, 현재 점은 바깥쪽 -> 교차점 추가
            else if (dist_prev >= 0 && dist_current <= 0)
                new_list_vertices.push_back(IntersectPlane(prev_v, current_v, plane));
            // Case 3: 이전 점은 바깥쪽, 현재 점은 안쪽 -> 교차점과 현재 점 추가
            else if (dist_prev <= 0 && dist_current >= 0)
            {
                new_list_vertices.push_back(IntersectPlane(prev_v, current_v, plane));
                new_list_vertices.push_back(current_v);
            }
            // Case 4: 두 점 모두 바깥쪽 -> 아무것도 추가 안함
        }
        list_vertices = new_list_vertices;
    }
    // 클리핑 결과로 남은 다각형(convex polygon)을 삼각형으로 분할 (Triangle Fan)
    if (list_vertices.size() >= 3)
    {
        Vertex first_vertex = list_vertices[0];
        for (size_t i = 1; i < list_vertices.size() - 1; ++i)
        {
            Triangle new_triangle;
            new_triangle.vertices[0] = first_vertex;
            new_triangle.vertices[1] = list_vertices[i];
            new_triangle.vertices[2] = list_vertices[i + 1];
            out_triangles.push_back(new_triangle);
        }
    }
}
