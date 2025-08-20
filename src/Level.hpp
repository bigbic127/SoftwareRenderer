#pragma once
#include "GltfData.hpp"
#include "Mesh.hpp"
#include <vector>

struct Level
{
    vector<Node> nodes;
    vector<Mesh> meshes;
    vector<Material> materials;
    vector<Texture> textures;
    vector<Image> images;
    vector<uint32_t> path;
};