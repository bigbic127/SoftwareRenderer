#pragma once

#include <vector>
#include "Vector.hpp"

template<typename T>
struct Keyframe
{
    float time;
    T value;
};

struct AnimationSampler
{
    std::string interpolation;
    std::vector<Keyframe<Vector3>> translationKeys;
    std::vector<Keyframe<Quaternion>> rotationKeys;
    std::vector<Keyframe<Vector3>> scaleKeys;
};

struct AnimationChannel
{
    int targetNode;
    std::string targetPath;
    AnimationSampler sampler;
};

struct Animation
{
    std::string name;
    std::vector<AnimationChannel> channels;
};