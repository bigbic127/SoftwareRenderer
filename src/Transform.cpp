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