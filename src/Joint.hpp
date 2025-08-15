#pragma once

#include <vector>
#include "Vector.hpp"
#include "Matrix.hpp"

using namespace std;

struct Bone {
    std::string name;
    int parent = -1;
    std::vector<int> children;
    Vector3 translation;
    Vector4 rotation;
    Vector3 scale;
    Matrix4x4 inverseBindMatrix;
};

template<typename T>
struct Keyframe {
    float time;
    T value;
};

struct AnimationSampler {
    std::string interpolation;
    std::vector<Keyframe<Vector3>> translationKeys;
    std::vector<Keyframe<Vector4>> rotationKeys;
    std::vector<Keyframe<Vector3>> scaleKeys;
};

struct AnimationChannel {
    int targetNode;
    std::string targetPath;
    AnimationSampler sampler;
};

struct Animation {
    std::string name;
    std::vector<AnimationChannel> channels;
};