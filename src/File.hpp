#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <tiny_gltf.h>

#include <SDL.h>
#include "Matrix.hpp"
#include "Mesh.hpp"
#include "Vector.hpp"
#include "lodepng.h"
#include "Node.hpp"
#include "Scene.hpp"

using namespace std;

static void LoadPngFile(const string& path, Mesh& mesh)
{
    vector<uint32_t>& texture = mesh.GetTexture();
    int& width = mesh.GetUVsWidth();
    int& height = mesh.GetUVsHeight();
    std::vector<unsigned char> image;
    unsigned _width;
    unsigned _height;
    unsigned error = lodepng::decode(image, _width, _height, path);
    if (error)
        SDL_Log("PNG Load Error");


    size_t pixelCount = _width * _height;
    width = int(_width);
    height = int(_height);
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
    vector<Vertex>& vertex = mesh.GetVertex();
    vector<Vector3i>& indices = mesh.GetIndices();
    vector<Vector3> vertices;
    vector<Vector3> normals;
    vector<Vector2> uvs;
    vector<Vector3i> indices_nor;
    vector<Vector3i> indices_uv;    
    vertex.clear();
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
            if (line.find('/') != std::string::npos)
            {
                int vIdx[3], vtIdx[3], vnIdx[3];
                char slash;
                for (int i = 0; i < 3; ++i) {
                    iss >> vIdx[i] >> slash >> vtIdx[i] >> slash >> vnIdx[i];
                }
                Vector3i point;
                Vector3i normal;
                Vector3i uv;
                for (int i = 0; i < 3; ++i) {
                    int v = vIdx[i] - 1;
                    int n = vnIdx[i] - 1;
                    int u = vtIdx[i] - 1;
                    if (i == 0)
                    {
                        point.a = v;
                        normal.a = n;
                        uv.a = u;
                    }
                    else if (i == 1)
                    {
                        point.b = v;
                        normal.b = n;
                        uv.b = u;
                    }
                    else
                    {
                        point.c = v;
                        normal.c = n;
                        uv.c = u;
                    }
                }
                indices.push_back(point);
                indices_nor.push_back(normal);
                indices_uv.push_back(uv);
            }else
            {
                Vector3i f;
                iss >> f.a >> f.b >> f.c;
                f.a -=1;
                f.b -=1;
                f.c -=1;
                indices.push_back(f);
            }
        }
    }
    //indices에 맞게 UV,Normal 수정
    vector<Vector3> newNormals;
    vector<Vector2> newuvs;
    newNormals.resize(vertices.size());
    newuvs.resize(vertices.size());
    for (size_t i = 0; i < indices.size(); i++)
    {
        Vector3i iv = indices[i];
        Vector3i in = indices_nor[i];
        Vector3i iu = indices_uv[i];
        Vector3 nn1 = normals[in.a];
        Vector3 nn2 = normals[in.b];
        Vector3 nn3 = normals[in.c];
        Vector2 un1 = uvs[iu.a];
        Vector2 un2 = uvs[iu.b];
        Vector2 un3 = uvs[iu.c];
        newNormals[iv.a] = nn1;
        newNormals[iv.b] = nn2;
        newNormals[iv.c] = nn3;
        newuvs[iv.a] = un1;
        newuvs[iv.b] = un2;
        newuvs[iv.c] = un3;
    }
    normals = newNormals;
    uvs = newuvs;
    //Vertex 구조
    for (size_t i = 0; i < vertices.size(); i++)
    {
        Vertex v;
        v.pos = Vector4(vertices[i]);
        v.nor = Vector4(normals[i], 0.0f);
        v.uv = uvs[i];
        vertex.push_back(v);
    }
}


Scene LoadGLTF(std::string filename)
{
    Scene scene;
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;
    bool res = loader.LoadBinaryFromFile(&model, &err, &warn, filename.c_str());//LoadASCIIFromFile
    if (!res)
        return scene;
    for (const auto& node : model.nodes) {
        Node _node;
        _node.name = node.name;
        _node.mesh = node.mesh;
        _node.skin = node.skin;
        if (!node.children.empty()) _node.children = node.children;
        if (!node.translation.empty()) _node.transform.SetPosition(Vector3(node.translation[0], node.translation[1], node.translation[2]));
        if (!node.rotation.empty()) _node.transform.SetQuterian(Quaternion(node.rotation[0], node.rotation[1], node.rotation[2], node.rotation[3]));
        if (!node.scale.empty()) _node.transform.SetScale(Vector3(node.scale[0], node.scale[1], node.scale[2]));
        if (!node.matrix.empty())
        {
            Matrix4x4 m;
            for (int row = 0; row < 4; row++)
                for (int col = 0; col < 4; col++)
                    m.m[row][col] = node.matrix[col * 4 + row]; 
            _node.matrix = m;
        }
        scene.nodes.push_back(_node);
    }
    for (const auto& mesh : model.meshes)
    {
        std::cout << "Processing Mesh: " << (mesh.name.empty() ? "(unnamed)" : mesh.name) << std::endl;
        for (const auto& primitive : mesh.primitives)
        {
            Mesh _mesh;
            vector<Vertex>& vertex = _mesh.GetVertex();
            vector<Vector3i>& indices = _mesh.GetIndices();
            vector<Vector3> vertices ;
            vector<Vector3> normals;
            vector<Vector2> uvs;
            vector<Vector4> weights;
            vector<Vector4i> joints;
            vertex.clear();
            indices.clear();

            std::string name = mesh.name;
            if (primitive.attributes.count("POSITION"))
            {
                const auto& accessor = model.accessors[primitive.attributes.at("POSITION")];
                const auto& bufferView = model.bufferViews[accessor.bufferView];
                const auto& buffer = model.buffers[bufferView.buffer];
                const float* pos_data = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
                vertices.reserve(accessor.count);
                for (size_t i = 0; i < accessor.count; ++i) {
                    vertices.push_back({pos_data[i * 3 + 0], pos_data[i * 3 + 1], pos_data[i * 3 + 2]});
                }
            }
            if (primitive.indices > -1)
            {
                const auto& accessor = model.accessors[primitive.indices];
                const auto& bufferView = model.bufferViews[accessor.bufferView];
                const auto& buffer = model.buffers[bufferView.buffer];
                const void* index_data_ptr = &buffer.data[bufferView.byteOffset + accessor.byteOffset];
                size_t Vector3i_count = accessor.count / 3;
                indices.reserve(Vector3i_count);
                switch (accessor.componentType) {
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE: {
                        const uint8_t* data = static_cast<const uint8_t*>(index_data_ptr);
                        for (size_t i = 0; i < Vector3i_count; ++i) {
                            indices.push_back({(int)data[i * 3 + 0], (int)data[i * 3 + 1], (int)data[i * 3 + 2]});
                        }
                        break;
                    }
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: {
                        const uint16_t* data = static_cast<const uint16_t*>(index_data_ptr);
                        for (size_t i = 0; i < Vector3i_count; ++i) {
                            indices.push_back({(int)data[i * 3 + 0], (int)data[i * 3 + 1], (int)data[i * 3 + 2]});
                        }
                        break;
                    }
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT: {
                        const uint32_t* data = static_cast<const uint32_t*>(index_data_ptr);
                        for (size_t i = 0; i < Vector3i_count; ++i) {
                            indices.push_back({(int)data[i * 3 + 0], (int)data[i * 3 + 1], (int)data[i * 3 + 2]});
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
            if (primitive.attributes.count("NORMAL"))
            {
                const auto& accessor = model.accessors[primitive.attributes.at("NORMAL")];
                const auto& bufferView = model.bufferViews[accessor.bufferView];
                const auto& buffer = model.buffers[bufferView.buffer];
                const float* data = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
                for (size_t v = 0; v < accessor.count; ++v) {
                    normals.push_back({data[v * 3 + 0], data[v * 3 + 1], data[v * 3 + 2]});
                }
            }
            if (primitive.attributes.count("TEXCOORD_0"))
            {
                const auto& accessor = model.accessors[primitive.attributes.at("TEXCOORD_0")];
                const auto& bufferView = model.bufferViews[accessor.bufferView];
                const auto& buffer = model.buffers[bufferView.buffer];
                const float* uv_data = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
                uvs.reserve(accessor.count);
                for (size_t i = 0; i < accessor.count; ++i) {
                    uvs.push_back({uv_data[i * 2 + 0], uv_data[i * 2 + 1]});
                }
            }
            // Vertex 구조
            for (size_t i = 0; i < vertices.size(); i++)
            {
                Vertex _v;                
                _v.pos = Vector4(vertices[i]);
                _v.nor = Vector4(normals[i], 0.0f);
                _v.uv = uvs[i];
                vertex.push_back(_v);
            }
            scene.meshes.push_back(_mesh);
        }
    }
    return scene;
}