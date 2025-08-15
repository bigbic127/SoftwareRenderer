#pragma once

#include <vector>
#include <string>
#include "Vector.hpp"
#include "Transform.hpp"
#include "Joint.hpp"

using namespace std;

class Mesh
{
    public:
        Mesh() {Cube();}
        vector<Vector3>& GetVertices() {return vertices;}
        vector<Vector3i>& GetIndices() {return indices;}
        vector<Vector3i>& GetNormalIndices() {return normalIndices;}
        vector<Vector3i>& GetUVIndices() {return uvIndices;}
        vector<Vector3>& GetNormals() {return normals;}
        vector<Vector2>& GetUVs() {return uvs;}
        vector<Triangle>& GetTriangle() {return triangles;}
        vector<Vector4>& GetWeights() {return Weights;}
        vector<Vector4i>& GetJoints() {return joints;}
        vector<Animation>& GetAnimation() {return animation;}
        int& GetUVsWidth() {return texWidth;}
        int& GetUVsHeight() {return texHeight;}
        vector<uint32_t>& GetColors() {return colors;}
        vector<uint32_t>& GetTexture() {return textures;}
        vector<Vector3>& GetPojectionPoint() {return projectionPoints;}
        Transform& GetTransform() {return transform;}
        vector<Vector3>& GetWorldVertices() {return worldVertices;}
        void Cube();
        void Sphere(int stacks = 32, int slices = 32, float radius = 1.0f);
    private:
        std::string meshName;
        std::string nodeName;
        int chNode, pNode;
        vector<Vector3> vertices;
        vector<Vector3i> indices;
        vector<Vector3> normals;
        vector<Vector2> uvs;
        vector<Vector3i> normalIndices;
        vector<Vector3i> uvIndices;
        vector<uint32_t> colors;
        Transform transform;
        vector<Vector3> projectionPoints;
        vector<Vector3> worldVertices;
        vector<uint32_t> textures;
        int texWidth, texHeight;
        vector<Vector4i> joints;
        vector<Vector4> Weights;
        vector<Animation> animation;
        vector<Triangle> triangles;
};