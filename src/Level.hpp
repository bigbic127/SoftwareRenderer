#pragma once
#include "GltfData.hpp"
#include "Mesh.hpp"
#include <vector>

struct Level
{
    vector<Node> nodes;
    vector<Mesh> meshes;
    vector<Material> materials;
};