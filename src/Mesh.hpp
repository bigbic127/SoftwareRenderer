#pragma once

#include <vector>
#include <string>
#include "GltfData.hpp"
#include "Vector.hpp"
#include "Transform.hpp"

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
        uint32_t color = 0xFF555555;
        Vector3 proj_m;
        Vector4 proj_clip;
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
        vector<Triangle>& GetTriangles() {return triangles;}
        vector<Vertex>& GetVertex() {return vertex;}
        vector<Vector3i>& GetIndices() {return indices;}
        Transform& GetTransform() {return transform;}
        vector<uint32_t>& GetTexture() {return textures;}
        int& GetUVsWidth() {return texWidth;}
        int& GetUVsHeight() {return texHeight;}
        uint32_t& GetColor() {return color;}
        void Cube();
        void Sphere(int stacks = 32, int slices = 32, float radius = 1.0f);
    public:
        int MaterialID;
        int parent = -1;
        int nodeIdex = -1;
        string name;
        int floatColor = 1;
    private:
        vector<Triangle> triangles;
        vector<Vertex> vertex;
        vector<Vector3i> indices;
        Transform transform;
        uint32_t color = 0xFF555555;
        vector<uint32_t> textures;
        int texWidth, texHeight;
        
};