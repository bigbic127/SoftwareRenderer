#pragma once

#include "Vector.hpp"

class Transform
{
    public:
        const Vector3D GetPosition() const {return position;}
        const Vector3D GetRotation() const {return rotation;}
        const Vector3D GetScale() const {return scale;}

        void SetPosition(Vector3D vec) {position = vec;}
        void SetRotation(Vector3D vec) {rotation = vec;}
        void SetScale(Vector3D vec) {rotation = vec;}

        Vector3D AddRotation_X(Vector3D vec, float angle);
        Vector3D AddRotation_Y(Vector3D vec, float angle);
        Vector3D AddRotation_Z(Vector3D vec, float angle);

    private:
        Vector3D position = {0,0,0};
        Vector3D rotation = {0,0,0};
        Vector3D scale = {1,1,1};

};