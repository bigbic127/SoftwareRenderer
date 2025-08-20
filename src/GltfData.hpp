#pragma once
#include <string>
#include <vector>
#include "Vector.hpp"
#include "Matrix.hpp"
#include "Transform.hpp"
#include <tiny_gltf.h>

struct Scene    
{
    std::string name;
    std::vector<int> nodes;
};

struct Node
{
    std::string name;
    int mesh = -1;
    int skin = -1;
    std::vector<int> children;
    Transform transform;
    int parent = -1;
};

struct Material
{
    bool doubleSided = true;
    std::string name;
    uint32_t  baseColorFactor;
    tinygltf::TextureInfo baseColorTexture;
    float metallicFactor = 0.0f;
    float roughnessFactor = 0.0f;
};

struct Texture
{
    int source;
    int sampler;
};

struct Image
{
    std::string name;
    std::string mimeType;
    std::vector<uint32_t> image;
    std::string url;
    int width;
    int height;
};


static uint32_t RGBAtoOx(std::vector<double> rgba)
{
    uint8_t R = static_cast<uint8_t>(rgba[0] * 255.0f);
    uint8_t G = static_cast<uint8_t>(rgba[1] * 255.0f);
    uint8_t B = static_cast<uint8_t>(rgba[2] * 255.0f);
    uint8_t A = static_cast<uint8_t>(rgba[3] * 255.0f);
    uint32_t color_ARGB = (A << 24) | (R << 16) | (G << 8) | B;
    uint32_t color_RGBA = (R << 24) | (G << 16) | (B << 8) | A;
    return color_ARGB;
}
