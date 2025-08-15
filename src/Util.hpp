#pragma once
#include "Vector.hpp"
#include <vector>


static Vertex IntersectVertex(const Vertex& v1, const Vertex& v2, const Vector4& plane)
{
    auto dist = [&](const Vertex& v) {
        return plane.x * v.pos.x +
               plane.y * v.pos.y +
               plane.z * v.pos.z +
               plane.w * v.pos.w;
    };

    float d1 = dist(v1);
    float d2 = dist(v2);
    float t = d1 / (d1 - d2);
    Vertex result;
    result.pos = v1.pos + ((v2.pos - v1.pos) * t);
    result.uv =  v1.uv + ((v2.uv - v1.uv) * t);
    return result;
}

static std::vector<Triangle> TriangulatePolygon(const std::vector<Vertex>& poly) {
    std::vector<Triangle> tris;
    for (size_t i = 1; i + 1 < poly.size(); ++i) {
        tris.push_back({ poly[0], poly[i], poly[i+1] });
    }
    return tris;
}
