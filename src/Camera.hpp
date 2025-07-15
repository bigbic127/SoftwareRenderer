#pragma once

#include "Vector.hpp"

class Camera
{
    public:
        Vector2D GetProject(Vector3D vec3);
        void SetPosition(Vector3D pos) {position = pos;}
        const Vector3D GetPosition() const {return position;}
        void SetFov(float fov) {fovValue = fov;}
        const float GetFov() const {return fovValue;}
    private:
        float fovValue = 500.0f;
        Vector3D position = {0, -0.25, -5};
};
