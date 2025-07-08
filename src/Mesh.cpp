#include "Mesh.hpp"

Mesh::Mesh()
{
    verticeNum = 8;
    vertices = new Vector3D[8];
    indices = new Vector3D[12];
}