#pragma once

#include "Matrix.hpp"
#include "Vector.hpp"

#define DEG2RAD(x) ((x) * 3.14159265359f / 180.0f)

class Transform
{
    public:
        Vector3 GetPosition()const{return position;}
        Vector3 GetRotation()const{return rotation;}
        Vector3 GetScale()const{return scale;}
        Quaternion GetQuterian() {return quterian;}
        void SetPosition(Vector3 pos) {this->position = pos;}
        void SetRotation(Vector3 rot) {this->rotation = rot;}
        void SetScale(Vector3 s) {this->scale = s;}
        void SetQuterian(Quaternion q) {this->quterian = q;}

        Vector3 GetRotateAroundAxis(const Vector3& v, const Vector3& axis, float angleRad)
        {
            Vector3 k = axis.Normalized();
            float cosTheta = cosf(angleRad);
            float sinTheta = sinf(angleRad);
            return v * cosTheta + k.Cross(v) * sinTheta + k * k.Dot(v) * (1 - cosTheta);
        }
        Matrix4x4 GetMatrix(){return Translation() * Rotation() * Scale();}
        Matrix4x4 GetQuatMatrix(){return Translation() * QuatRotation() * Scale();}

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
            float cp = cosf(DEG2RAD(rotation.x)), sp = sinf(DEG2RAD(rotation.x));
            float cy = cosf(DEG2RAD(rotation.y)), sy = sinf(DEG2RAD(rotation.y));
            float cr = cosf(DEG2RAD(rotation.z)), sr = sinf(DEG2RAD(rotation.z));
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
        Matrix4x4 QuatRotation()
        {
            Quaternion q = quterian.Normalized();
            double x2 = q.x * q.x;
            double y2 = q.y * q.y;
            double z2 = q.z * q.z;
            double xy = q.x * q.y;
            double xz = q.x * q.z;
            double yz = q.y * q.z;
            double wx = q.w * q.x;
            double wy = q.w * q.y;
            double wz = q.w * q.z;
            Matrix4x4 m;
            // 1열
            m.m[0][0] = 1.0 - 2.0 * (y2 + z2);
            m.m[0][1] = 2.0 * (xy + wz);
            m.m[0][2] = 2.0 * (xz - wy);
            m.m[0][3] = 0.0;
            // 2열
            m.m[1][0] = 2.0 * (xy - wz);
            m.m[1][1] = 1.0 - 2.0 * (x2 + z2);
            m.m[1][2] = 2.0 * (yz + wx);
            m.m[1][3] = 0.0;
            // 3열
            m.m[2][0] = 2.0 * (xz + wy);
            m.m[2][1] = 2.0 * (yz - wx);
            m.m[2][2] = 1.0 - 2.0 * (x2 + y2);
            m.m[2][3] = 0.0;
            // 4열
            m.m[3][0] = 0.0;
            m.m[3][1] = 0.0;
            m.m[3][2] = 0.0;
            m.m[3][3] = 1.0;
            return m;
        }


        Matrix4x4 Scale()
        {
            Matrix4x4 mat;
            mat.m[0][0] = scale.x;
            mat.m[1][1] = scale.y;
            mat.m[2][2] = scale.z;
            return mat;
        }
    public:
        Vector3 position;
        Vector3 rotation;
        Quaternion quterian;
        Vector3 scale = Vector3(1.0f, 1.0f ,1.0f);
};