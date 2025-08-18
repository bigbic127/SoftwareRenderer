#pragma once
#include "Node.hpp"
#include "Mesh.hpp"
#include <vector>

struct Scene
{
    vector<Node> nodes;
    vector<Mesh> meshes;
};