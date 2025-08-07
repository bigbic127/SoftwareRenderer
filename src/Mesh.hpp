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
        vector<Triangle>& GetNormalIndices() {return normalIndices;}
        vector<Triangle>& GetUVIndices() {return uvIndices;}
        vector<Vector3>& GetNormals() {return normals;}
        vector<Vector2>& GetUVs() {return uvs;}
        vector<uint32_t>& GetColors() {return colors;}
        vector<uint32_t>& GetTexture() {return colors;}
        
        Transform& GetTransform() {return transform;}
        vector<Vector3>& GetWorldVertices() {return worldVertices;}
        void Cube();
        void Sphere(int stacks = 32, int slices = 32, float radius = 1.0f);
    private:
        vector<Vector3> vertices;
        vector<Triangle> indices;
        vector<Vector3> normals;
        vector<Vector2> uvs;
        vector<Triangle> normalIndices;
        vector<Triangle> uvIndices;
        vector<uint32_t> colors;
        Transform transform;
        vector<Vector3> projectionPoints;
        vector<Vector3> worldVertices;
        vector<uint32_t> textures;
        int texWidth, texHeight;
};