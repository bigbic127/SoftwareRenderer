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
        float fovValue = 15000.0f; //500.0f
        Vector3D position = {0, -0.1, -5};// -1.5, -5
};
