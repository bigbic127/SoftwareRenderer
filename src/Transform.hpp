#pragma once

#include "Matrix.hpp"
#include "vector"

class Transform
{

    public:
        void SetPosition(Vector3 pos) {position = pos;}
        void SetRotation(vector3 rot) {rotation = rot;}
        void SetScale(Vector3 s) {scale = s;}
        
        Matrix4x4 GetTransform(){return Translation * (RotationZ * RotationY * RotationX) * Scale;}
    private:
        Matrix4x4 Translation()
        {
            Matrix4x4 mat;
            mat.m[0][3] = position.x;
            mat.m[1][3] = position.y;
            mat.m[2][3] = position.z;
        }
        Matrix4x4 Rotation()
        {
            return RotationZ(rotation.z) * RotationY(rotation.y) * RotationX(rotation.x);
        }

        Matrix4x4 Scale()
        {
            Matrix4x4 mat;
            mat.m[0][0] = scale.x;
            mat.m[1][1] = scale.y;
            mat.m[2][2] = scale.z;
            return mat;
        }
        Matrix4x4 RotationX(float x)
        {
            Matrix4x4 mat;
            float c = cos(x);
            float s = sin(x);
            mat.m[1][1] = c;
            mat.m[1][2] = -s;
            mat.m[2][1] = s;
            mat.m[2][2] = c;
            return mat;
        }
        Matrix4x4 RotationY(float y)
        {
            Matrix4x4 mat;
            float c = cos(y);
            float s = sin(y);
            mat.m[0][0] = c;
            mat.m[0][2] = s;
            mat.m[2][0] = -s;
            mat.m[2][2] = c;
            return mat;
        }
        Matrix4x4 RotationZ(float z)
        {
            Matrix4x4 mat;
            float c = cos(z);
            float s = sin(z);
            mat.m[0][0] = c;
            mat.m[0][1] = -s;
            mat.m[1][0] = s;
            mat.m[1][1] = c;
            return mat;
        }

    public:
        Vector3 position;
        Vector3 rotation;
        Vector3 scale;
};