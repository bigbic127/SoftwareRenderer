
#pragma once
#include <math.h>

struct Vector2D
{
    float x;
    float y;
};

struct Vector3D
{
    float x;
    float y;
    float z;
};

struct Triangle
{
    int x;
    int y;
    int z;
};


static Vector3D GetCrossVector(Vector3D vec1, Vector3D vec2)
{
    Vector3D vec;
    vec.x = vec1.y * vec2.z - vec1.z * vec2.y;
    vec.y = vec1.z * vec2.x - vec1.x * vec2.z;
    vec.z = vec1.x * vec2.y - vec1.y * vec2.x;
    return vec;
}

static float GetDotVector(Vector3D vec1, Vector3D vec2) {
    return (vec1.x * vec2.x) + (vec1.y * vec2.y) + (vec1.z * vec2.z);
}

static void Normalize(Vector3D& vec) {
    float length = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    vec.x /= length;
    vec.y /= length;
    vec.z /= length;
}

static Vector3D AddVector(Vector3D a, Vector3D b) {
    Vector3D result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

static Vector3D SubVector(Vector3D a, Vector3D b) {
    Vector3D result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}
