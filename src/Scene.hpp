#pragma once
#include "Node.hpp"
#include "Mesh.hpp"
#include "Joint.hpp"
#include "Animation.hpp"
#include <vector>

class Scene
{
    private:
        vector<Node> nodes;
        vector<Mesh> meshes;
        vector<Joint> joints;
        vector<Animation> animations;
};