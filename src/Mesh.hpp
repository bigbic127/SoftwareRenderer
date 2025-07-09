#pragma once

#include <vector>

#include "Transform.hpp"
#include "Vector.hpp"

using namespace std;

class Mesh: public Transform
{
    public:
        Mesh();
        const int GetVerticeNum() const {return verticeNum;}
        const int GetIndicesNum() const {return indicesNum;}
        const Vector3D* GetVertices() const {return vertices;}
        Triangle* GetIndices() {return indices;}
        Vector2D* GetProjectPoints() {return projectPoints;}


    private:
        int verticeNum = 0;
        int indicesNum = 0;
        Vector3D* vertices;
        Triangle* indices;
        Vector2D* projectPoints;
};