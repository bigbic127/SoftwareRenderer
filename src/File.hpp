#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#include <SDL.h>

#include "Mesh.hpp"
#include "Vector.hpp"
#include "lodepng.h"

using namespace std;

static void LoadPngFile(const string& path, Mesh& mesh)
{
    vector<uint32_t>& texture = mesh.GetTexture();
    std::vector<unsigned char> image;
    unsigned width;
    unsigned height;
    unsigned error = lodepng::decode(image, width, height, path);
    if (error)
        SDL_Log("PNG Load Error");


    size_t pixelCount = width * height;
    texture.clear();
    for (size_t i = 0; i < pixelCount; ++i) {
        uint8_t r = image[i * 4 + 0];
        uint8_t g = image[i * 4 + 1];
        uint8_t b = image[i * 4 + 2];
        uint8_t a = image[i * 4 + 3];

        // AARRGGBB 포맷으로 저장
        texture.push_back((a << 24) | (r << 16) | (g << 8) | b);
    }
}

static void LoadObjFile(const string& path, Mesh& mesh)//, vector<Vector2D>& uvs, vector<Vector3D>& normals)
{
    ifstream file(path);
    if (!file.is_open())
    {
        SDL_Log("Faild to open file. path:%s\n", path.c_str());
        return;
    }
    vector<Vector3>& vertices = mesh.GetVertices();
    vector<Triangle>& indices = mesh.GetIndices();
    vector<Triangle>& normalIndices = mesh.GetNormalIndices();
    vector<Triangle>& uvIndices = mesh.GetUVIndices();
    vector<Vector3>& normals = mesh.GetNormals();
    vector<Vector2>& uvs = mesh.GetUVs();

    vertices.clear();
    indices.clear();
    normalIndices.clear();
    uvIndices.clear();
    normals.clear();
    uvs.clear();
    
    string line;
    while(getline(file, line))
    {
        string prefix;
        istringstream iss(line);
        //SDL_Log("Line: %s\n", line.c_str());
        iss >> prefix;
        if (prefix == "v")
        {
            Vector3 v;
            iss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        }
        else if (prefix == "vn")
        {
            Vector3 nor;
            iss >> nor.x >> nor.y >> nor.z;
            normals.push_back(nor);
        }
        else if (prefix == "vt")
        {
            Vector2 uv;
            iss >> uv.x >> uv.y;
            uvs.push_back(uv);
        }
        else if (prefix == "f")
        {
            Triangle f;
            if (line.find('/') != std::string::npos)
            {
                int vIdx[3], vtIdx[3], vnIdx[3];
                char slash;
                for (int i = 0; i < 3; ++i) {
                    iss >> vIdx[i] >> slash >> vtIdx[i] >> slash >> vnIdx[i];
                }
                Triangle vertex;
                Triangle normal;
                Triangle uv;
                for (int i = 0; i < 3; ++i) {
                    //v.position = positions[vIdx[i] - 1];
                    //v.uv       = texcoords[vtIdx[i] - 1];
                    //v.normal   = normals[vnIdx[i] - 1];
                    int v = vIdx[i] - 1;
                    int n = vnIdx[i] - 1;
                    int u = vtIdx[i] - 1;
                    if (i == 0)
                    {
                        vertex.a = v;
                        normal.a = n;
                        uv.a = u;
                    }
                    else if (i == 1)
                    {
                        vertex.b = v;
                        normal.b = n;
                        uv.b = u;
                    }
                    else
                    {
                        vertex.c = v;
                        normal.c = n;
                        uv.c = u;
                    }
                }
                indices.push_back(vertex);
                normalIndices.push_back(normal);
                uvIndices.push_back(uv);                
            }else
            {
                iss >> f.a >> f.b >> f.c;
                f.a -=1;
                f.b -=1;
                f.c -=1;
                indices.push_back(f);
            }
        }
    }    
}