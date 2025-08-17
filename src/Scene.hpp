#pragma once
#include "Node.hpp"
#include "Mesh.hpp"
#include "Joint.hpp"
#include "Animation.hpp"
#include <vector>

struct Scene
{
    vector<Node> nodes;
    vector<Mesh> meshes;
    vector<Joint> joints;
    vector<Animation> animations;
};