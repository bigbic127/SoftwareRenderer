#pragma once

#include <vector>
#include <string>
#include "Node.hpp"
#include "Vector.hpp"
#include "Transform.hpp"
#include "Joint.hpp"
#include "Animation.hpp"

using namespace std;


class Vertex
{
    public:
        Vertex(){}
        Vertex(Vector4 p, Vector2 u) : pos(p), uv(u){}
    public:
        Vector4 pos;
        Vector4 nor;
        Vector2 uv;
        vector<int> jointIndices;
        vector<float> weights;

};

class Triangle
{
    public:
        Triangle(){}
        Triangle(Vertex v1, Vertex v2, Vertex v3)
        {
            vertices[0] = v1;
            vertices[1] = v2;
            vertices[2] = v3;
        }
    public:
        Vertex vertices[3];
};


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
        vector<Joint>& GetJoints() {return joints;}
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
        Node node;
        vector<Triangle> triangles;
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
        vector<Joint> joints;
        vector<Animation> animation;
};