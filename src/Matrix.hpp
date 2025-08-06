#pragma once

#include "Vector.hpp"

class Matrix4x4
{
    public:
        Matrix4x4()
        {
            Identity();
        }

        void Identity()
        {
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    m[i][j] = (i == j) ? 1.0f : 0.0f;
        }

        Matrix4x4 operator*(const Matrix4x4& mat) const
        {
            Matrix4x4 result;
            for (int row = 0; row < 4; ++row)
            {
                for (int col = 0; col <4; ++col)
                {
                    float sum = 0.0f;
                    for(int k = 0; k < 4; ++k)
                    {
                        sum += m[row][k] * mat.m[k][col];
                    }
                    result.m[row][col] = sum;
                }
            }
            return result;
        }

        Vector3 operator*(const Vector3& v) const
        {
            float x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3];
            float y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3];
            float z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3];
            return Vector3(x, y, z);
        }

        Vector4 operator*(const Vector4& v) const
        {
            float x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
            float y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
            float z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
            float w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;
            return Vector4(x, y, z, w);
        }
        Vector4 GetRow(int row) const
        {
            return Vector4(m[row][0], m[row][1], m[row][2], m[row][3]);
        }
    public:
        float m[4][4];
};