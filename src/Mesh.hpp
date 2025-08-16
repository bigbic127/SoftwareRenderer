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
        Vector4i jointIndices;
        Vector4 weights;
        Vector3 proj_m;
        Vector3 proj_p;
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
        void SetTriangles(vector<Triangle> tri) {triangles = tri;}
        vector<Triangle>& GetTriangles() {return triangles;}
        vector<Vector3>& GetVertices() {return vertices;}
        vector<Vector3i>& GetIndices() {return indices;}
        vector<Vector3>& GetNormals() {return normals;}
        vector<Vector2>& GetUVs() {return uvs;}
        vector<Vertex>& GetVertex() {return vertex;}
        Transform& GetTransform() {return transform;}
        vector<uint32_t>& GetTexture() {return textures;}
        int& GetUVsWidth() {return texWidth;}
        int& GetUVsHeight() {return texHeight;}
        uint32_t& GetColor() {return color;}
        void Cube();
        void Sphere(int stacks = 32, int slices = 32, float radius = 1.0f);
    private:
        Node node;
        // triangle (구조 파악용)
        vector<Triangle> triangles;
        //vertex, indices buffer
        vector<Vector3> vertices;
        vector<Vector3i> indices;
        vector<Vector3> normals;
        vector<Vector2> uvs;
        vector<Vertex> vertex;
        uint32_t color = 0xFF555555;
        Transform transform;
        vector<uint32_t> textures;
        int texWidth, texHeight;
        vector<Joint> joints;
        vector<Animation> animation;
};