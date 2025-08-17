#pragma once

#include <vector>
#include "Vector.hpp"
#include "Matrix.hpp"
#include "Transform.hpp"

struct Joint
{
    std::string name;
    int parent = -1;
    std::vector<int> children;
    std::vector<Matrix4x4> inverseBindMatrices;
};
