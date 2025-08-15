#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <tiny_gltf.h>

#include <SDL.h>

#include "Mesh.hpp"
#include "Vector.hpp"
#include "lodepng.h"

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


std::vector<Mesh> LoadGLTF(std::string filename)
{
    std::vector<Mesh> result;
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;
    bool res = loader.LoadBinaryFromFile(&model, &err, &warn, filename.c_str());//LoadASCIIFromFile
    if (!res)
        return result;

    for (const auto& mesh : model.meshes)
    {
        std::cout << "Processing Mesh: " << (mesh.name.empty() ? "(unnamed)" : mesh.name) << std::endl;
        for (const auto& primitive : mesh.primitives)
        {
            Mesh _mesh;
            vector<Vector3>& vertices = _mesh.GetVertices();
            vector<Triangle>& indices = _mesh.GetIndices();
            vector<Triangle>& normalIndices = _mesh.GetNormalIndices();
            vector<Triangle>& uvIndices = _mesh.GetUVIndices();
            vector<Vector3>& normals = _mesh.GetNormals();
            vector<Vector2>& uvs = _mesh.GetUVs();
            vector<Vector4>& weights = _mesh.GetWeights();
            vector<Vector4i>& joints = _mesh.GetJoints();
            vertices.clear();
            indices.clear();
            normalIndices.clear();
            uvIndices.clear();
            normals.clear();
            uvs.clear();
            weights.clear();
            joints.clear();
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
                size_t triangle_count = accessor.count / 3;
                indices.reserve(triangle_count);
                switch (accessor.componentType) {
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE: {
                        const uint8_t* data = static_cast<const uint8_t*>(index_data_ptr);
                        for (size_t i = 0; i < triangle_count; ++i) {
                            indices.push_back({(int)data[i * 3 + 0], (int)data[i * 3 + 1], (int)data[i * 3 + 2]});
                        }
                        break;
                    }
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: {
                        const uint16_t* data = static_cast<const uint16_t*>(index_data_ptr);
                        for (size_t i = 0; i < triangle_count; ++i) {
                            indices.push_back({(int)data[i * 3 + 0], (int)data[i * 3 + 1], (int)data[i * 3 + 2]});
                        }
                        break;
                    }
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT: {
                        const uint32_t* data = static_cast<const uint32_t*>(index_data_ptr);
                        for (size_t i = 0; i < triangle_count; ++i) {
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
            uvIndices =  indices;
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
            if (primitive.attributes.count("JOINTS_0"))
            {
                const auto& accessor = model.accessors[primitive.attributes.at("JOINTS_0")];
                const auto& bufferView = model.bufferViews[accessor.bufferView];
                const auto& buffer = model.buffers[bufferView.buffer];
                const void* data_ptr = &buffer.data[bufferView.byteOffset + accessor.byteOffset];
                switch (accessor.componentType) {
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: {
                        const uint16_t* data = static_cast<const uint16_t*>(data_ptr);
                        for (size_t v = 0; v < accessor.count; ++v) {
                            joints.push_back({data[v * 4 + 0], data[v * 4 + 1], data[v * 4 + 2], data[v * 4 + 3]});
                        }
                        break;
                    }
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE: {
                        const uint8_t* data = static_cast<const uint8_t*>(data_ptr);
                        for (size_t v = 0; v < accessor.count; ++v) {
                            joints.push_back({(unsigned short)data[v * 4 + 0], (unsigned short)data[v * 4 + 1], (unsigned short)data[v * 4 + 2], (unsigned short)data[v * 4 + 3]});
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
            if (primitive.attributes.count("WEIGHTS_0"))
            {
                const auto& accessor = model.accessors[primitive.attributes.at("WEIGHTS_0")];
                const auto& bufferView = model.bufferViews[accessor.bufferView];
                const auto& buffer = model.buffers[bufferView.buffer];
                const float* data = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
                for (size_t v = 0; v < accessor.count; ++v) {
                    weights.push_back({data[v * 4 + 0], data[v * 4 + 1], data[v * 4 + 2], data[v * 4 + 3]});
                }
            }
            result.push_back(_mesh);
        }
    }
    return result;
}