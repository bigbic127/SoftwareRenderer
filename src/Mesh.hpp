#pragma once

#include <vector>
#include "Vector.hpp"
#include "Transform.hpp"

using namespace std;

class Mesh
{
    public:
        Mesh() {Cube();}
        vector<Vector3>& GetVertices() {return vertices;}
        vector<Triangle>& GetIndices() {return indices;}
        vector<uint32_t>& GetColors() {return colors;}
        Transform& GetTransform() {return transform;}
        vector<Vector3>& GetWorldVertices() {return worldVertices;}
        void Cube();
    private:
        vector<Vector3> vertices;
        vector<Triangle> indices;
        vector<Vector3> normals;
        vector<Vector2> uvs;
        vector<uint32_t> colors;
        Transform transform;
        vector<Vector2> projectionPoints;
        vector<Vector3> worldVertices;
};