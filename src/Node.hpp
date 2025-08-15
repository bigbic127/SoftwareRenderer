#pragma once

#include "Vector.hpp"
#include "Matrix.hpp"
#include <vector>
#include <string>

struct Node
{
    std::string name;
    vector<int> children;
    Vector3 translation;
    Vector4 rotation;
    Vector3 scale;
    Matrix4x4 matrix;
};
