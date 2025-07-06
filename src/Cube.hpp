#pragma once

#include "Vector.hpp"
#include "Transform.hpp"

class Cube
{
    public:
        Cube();
        const int GetPointsNum() const {return cubePointsNum;}
        const Vector3D* GetPoints() const {return points;}
        Vector2D* GetProjectPoints() {return projectPotions;}
        const Transform GetTransform() const {return transform;}
        void SetTransform(Transform trans) {transform = trans;}

    private:
        static const int cubePointsNum = 9 * 9 * 9;
        Vector3D points[cubePointsNum];
        Vector2D projectPotions[cubePointsNum];

        Transform transform;
};