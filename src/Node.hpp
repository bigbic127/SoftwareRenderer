#pragma once
#include <vector>
#include <string>

#include "Vector.hpp"
#include "Matrix.hpp"
#include "Transform.hpp"

struct Node
{
    std::string name;
    int mesh = -1;
    int skin = -1;
    std::vector<int> children;
    Transform transform;
    Matrix4x4 matrix;
    Matrix4x4 localMatrix;
};
