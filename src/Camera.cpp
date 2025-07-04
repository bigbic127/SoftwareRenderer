#include "Camera.hpp"


Vector2D Camera::GetProject(Vector3D vec3)
{
    Vector2D vec2= {(fovValue * vec3.x) / vec3.z, (fovValue * vec3.y) / vec3.z};
    return vec2;
}
