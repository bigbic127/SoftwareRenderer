#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#include <SDL.h>

#include "Vector.hpp"

using namespace std;

static void LoadObjFile(const string& path, vector<Vector3>& vertices, vector<Triangle>& indices)//, vector<Vector2D>& uvs, vector<Vector3D>& normals)
{
    ifstream file(path);
    if (!file.is_open())
    {
        SDL_Log("Faild to open file. path:%s\n", path.c_str());
        return;
    }
    vertices.clear();
    indices.clear();
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
        else if (prefix == "f")
        {
            Triangle f;
            if (line.find('/') != std::string::npos)
            {
                
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