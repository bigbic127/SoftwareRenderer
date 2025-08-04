#pragma once

#include <cmath>

class Vector2
{
    public:
        Vector2() : x(0), y(0){}
        Vector2(float _x, float _y) : x(_x), y(_y){}
    public:
        float x, y;
};

class Vector3
{
    public:
        Vector3() : x(0), y(0), z(0) {}
        Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

        Vector3 operator+(const Vector3& vec) const
        {
            return Vector3(x + vec.x, y + vec.y, z + vec.z);
        }
        Vector3 operator-(const Vector3& vec) const
        {
            return Vector3(x - vec.x, y - vec.y, z - vec.z);
        }
        Vector3 operator*(float scalar) const
        {
            return Vector3(x * scalar, y * scalar, z * scalar);
        }
        Vector3 operator/(float scalar) const
        {
            return Vector3(x / scalar, y / scalar, z / scalar);
        }
        float Dot(const Vector3& vec) const
        {
            return x * vec.x + y * vec.y + z * vec.z;
        }
        Vector3 Cross(const Vector3& vec) const
        {
            return Vector3(
                y * vec.z - z * vec.y,
                z * vec.x - x * vec.z,
                x * vec.y - y * vec.x
            );
        }
        float Length() const
        {
            return std::sqrt(x*x + y*y + z*z);
        }
        Vector3 Normalized() const
        {
            float len = Length();
            if (len == 0) return Vector3(0,0,0);
            return (*this) / len;
        }
    public:
        float x, y, z;
};

class Vector4
{
    public:
        Vector4() : x(0), y(0), z(0), w(1){}
        Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w){}
    public:
        float x, y, z, w;
};

class Triangle
{
    public:
        Triangle() : a(0), b(0), c(0) {}
        Triangle(int _a, int _b, int _c) : a(_a), b(_b), c(_c) {}
    public:
        int a, b, c;
};