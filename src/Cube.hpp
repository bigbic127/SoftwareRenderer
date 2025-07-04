#pragma once

#include "Vector.hpp"

class Cube
{
    public:
        Cube();
        const int GetPointsNum() const {return cubePointsNum;}
        const Vector3D* GetPoints() const {return points;}
        Vector2D* GetProjectPoints() {return projectPotions;}
    private:
        static const int cubePointsNum = 9 * 9 * 9;
        Vector3D points[cubePointsNum];
        Vector2D projectPotions[cubePointsNum];
};