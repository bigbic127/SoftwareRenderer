#pragma once

#include <vector>
#include "Vector.hpp"
#include "Transform.hpp"

using namespace std;

class Mesh
{
    public:
        Mesh();
        vector<Vector3>& GetVertices() {return vertices;}
        vector<Triangle>& GetIndices() {return indices;}

    private:
        vector<Vector3> vertices;
        vector<Triangle> indices;
        vector<uint32_t> colors;
        Transform transform;
        vector<Vector2> projectPoint;
};