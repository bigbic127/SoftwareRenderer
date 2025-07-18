#pragma once

#include <vector>

#include "Transform.hpp"
#include "Vector.hpp"

using namespace std;

class Mesh: public Transform
{
    public:
        Mesh();
        vector<Vector3D>& GetVertices() {return vertices;}
        vector<Triangle>& GetIndices() {return indices;}
        vector<Vector2D>& GetProjectPoints() {return projectPoints;}
        vector<Vector3D>& GetNormals() {return normals;}
        vector<uint32_t>& GetColors() {return colors;}
        vector<Vector3D>& GetPoints() {return points;}



    private:
        vector<Vector3D> vertices;
        vector<Triangle> indices;
        vector<Vector2D> projectPoints;
        vector<Vector3D> normals;
        vector<uint32_t> colors;
        vector<Vector3D> points;

};