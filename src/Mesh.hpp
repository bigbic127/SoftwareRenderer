#pragma once
#include "Transform.hpp"
#include "Vector.hpp"

class Mesh: public Transform
{
    public:
        Mesh();

    private:
        int verticeNum = 0;
        Vector3D* vertices;
        Vector3D* indices;
};