#include "Vector.hpp"

class Cube
{
    public:
        Vector3D& GetPoints() const {return points;}
        void const SetPoints(Vector3D& vec3);
        Vector2D GetFromVec2(Vector3D& vec3);
    private:
        static const int cubePointNum = 9 * 9 * 9;
        Vector3D points[cubePointNum];
};

void const Cube::SetPoints(Vector3D& vec3)
{
    points = vec3;
}

Vector2D Cube::GetFromVec2(Vector3D& vec3)
{
    Vector2D vec2 = {vec3.x / vec3.z, vec3.y / vec3.z};
    return vec2;
}
