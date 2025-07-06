#include "Transform.hpp"

#include <math.h>


Vector3D Transform::AddRotation_X(Vector3D vec, float angle)
{
    Vector3D vector ={
        vec.x, 
        vec.y * cos(angle) - vec.z * sin(angle),
        vec.y * sin(angle) + vec.z * cos(angle),
    };
    return vector;
}

Vector3D Transform::AddRotation_Y(Vector3D vec, float angle)
{
    Vector3D vector ={
        vec.x * cos(angle) - vec.z * sin(angle), 
        vec.y,
        vec.x * sin(angle) + vec.z * cos(angle),
    };
    return vector;
}

Vector3D Transform::AddRotation_Z(Vector3D vec, float angle)
{
    Vector3D vector ={
        vec.x * cos(angle) - vec.y * sin(angle), 
        vec.x * sin(angle) + vec.y * cos(angle),
        vec.z
    };
    return vector;
}