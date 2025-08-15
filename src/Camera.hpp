#pragma once

#include "Matrix.hpp"
#include "Vector.hpp"
#include <vector>

using namespace std;

class Camera
{
    public:
        void SetLookAt(Vector3 _eye, Vector3 _target, Vector3 _up)
        {
            this->eye = _eye;
            this->target = _target;
            this->up = _up;
        }
        void SetPerspective(float _fov, float _aspect, float _near, float _far)
        {
            this->fov = _fov;
            this->aspect = _aspect;
            this->near = _near;
            this->far = _far;
        }
        Matrix4x4 GetProjectionMatrix() {return Perspective() * LookAt();}
        Matrix4x4 GetViewMatrix() {return LookAt();}
        Vector3 GetDirection() {return (target - eye).Normalized();}
        Vector3 GetPosition() const {return eye;}
        Vector3 GetTarget() const {return target;}
        vector<Vector4> GetFrustumPlanes() {return ExtractFrustumPlanes();}

    private:
        Matrix4x4 LookAt()
        {
            Vector3 f = (target - eye).Normalized();   // Forward
            Vector3 r = f.Cross(up).Normalized();      // Right
            Vector3 u = r.Cross(f).Normalized();       // Up
            Matrix4x4 view;
            view.m[0][0] = r.x;
            view.m[0][1] = r.y;
            view.m[0][2] = r.z;
            view.m[0][3] = r.Dot(eye)*-1;
            view.m[1][0] = u.x;
            view.m[1][1] = u.y;
            view.m[1][2] = u.z;
            view.m[1][3] = u.Dot(eye)*-1;
            view.m[2][0] = -f.x;
            view.m[2][1] = -f.y;
            view.m[2][2] = -f.z;
            view.m[2][3] = f.Dot(eye);
            return view;
        }
        Matrix4x4 Perspective()
        {
            float f = 1.0f / tan(fov * 0.5f);
            float nf = 1.0f / (near - far);
            Matrix4x4 mat;
            mat.m[0][0] = f / aspect;
            mat.m[1][1] = f;
            mat.m[2][2] = (far + near) * nf;
            mat.m[2][3] = (2 * far * near) * nf;
            mat.m[3][2] = -1.0f;
            mat.m[3][3] = 0.0f;
            return mat;
        }
        //절두체 6면
        vector<Vector4> ExtractFrustumPlanes()
        {
            Matrix4x4 m;
            planes.clear();
            planes.push_back((m.GetRow(3) + m.GetRow(0)).Normalized()); // Left
            planes.push_back((m.GetRow(3) - m.GetRow(0)).Normalized()); // Right
            planes.push_back((m.GetRow(3) + m.GetRow(1)).Normalized()); // Bottom
            planes.push_back((m.GetRow(3) - m.GetRow(1)).Normalized()); // Top
            planes.push_back((m.GetRow(3) + m.GetRow(2)).Normalized()); // Near
            planes.push_back((m.GetRow(3) - m.GetRow(2)).Normalized()); // Far
            return planes;
        }
        private:
            Vector3 eye;
            Vector3 target;
            Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
            float fov = 60.f;
            float aspect = 1.0f;
            float near = 0.1f;
            float far = 100.f;
            vector<Vector4> planes;
};