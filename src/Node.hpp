#pragma once
#include <vector>
#include <string>

#include "Vector.hpp"
#include "Matrix.hpp"
#include "Transform.hpp"

struct Node
{
    public:
        std::string name;
        std::vector<int> children;
        Transform transform;
        Matrix4x4 matrix;
};
