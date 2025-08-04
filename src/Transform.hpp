#pragma once

#include "Matrix.hpp"
#include "Vector.hpp"

#define DEG2RAD(x) ((x) * 3.14159265359f / 180.0f)

class Transform
{

    public:
        void SetPosition(Vector3 pos) {position = pos;}
        void SetRotation(Vector3 rot) {rotation = rot;}
        void SetScale(Vector3 s) {scale = s;}
        
        Matrix4x4 GetTransform(){return Translation() * Rotation() * Scale();}
    private:
        Matrix4x4 Translation()
        {
            Matrix4x4 mat;
            mat.m[0][3] = position.x;
            mat.m[1][3] = position.y;
            mat.m[2][3] = position.z;
            return mat;
        }
        Matrix4x4 Rotation()
        {
            Matrix4x4 rx, ry, rz;

            float cp = cosf(DEG2RAD(rotation.y)), sp = sinf(DEG2RAD(rotation.y));
            float cy = cosf(DEG2RAD(rotation.z)), sy = sinf(DEG2RAD(rotation.z));
            float cr = cosf(DEG2RAD(rotation.x)), sr = sinf(DEG2RAD(rotation.x));

            // X축 회전 (Pitch)
            rx.m[1][1] = cp;
            rx.m[1][2] = -sp;
            rx.m[2][1] = sp;
            rx.m[2][2] = cp;

            // Y축 회전 (Yaw)
            ry.m[0][0] = cy;
            ry.m[0][2] = sy;
            ry.m[2][0] = -sy;
            ry.m[2][2] = cy;

            // Z축 회전 (Roll)
            rz.m[0][0] = cr;
            rz.m[0][1] = -sr;
            rz.m[1][0] = sr;
            rz.m[1][1] = cr;
            
            return rz * ry * rx; 
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
        Vector3 scale = Vector3(1.0f, 1.0f ,1.0f);
};