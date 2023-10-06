#pragma once
#include "GLM.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"

namespace IW3SR
{
    /// <summary>
    /// 3x3 Matrix class.
    /// </summary>
    template <typename T>
    class Matrix3 : public gmat3<T>
    {
        using mat3 = Matrix3<T>;

    public:
        /// <summary>
        /// Initialize a 3x3 identity matrix.
        /// </summary>
        Matrix3() : gmat3<T>(1) { }

        /// <summary>
        /// Initialize a 3x3 matrix with specified values.
        /// </summary>
        /// <param name="m">An array of 9 values in row-major order.</param>
        Matrix3(T* m) : gmat3<T>(m) { }

        /// <summary>
        /// Initialize a 3x3 matrix with specified values.
        /// </summary>
        /// <param name="m">An array of 9 values in row-major order.</param>
        Matrix3(T m[3][3]) : gmat3<T>(vec3{ m[0] }, vec3{ m[0] }, vec3{ m[0] }) { }

        /// <summary>
        /// Transpose the matrix.
        /// </summary>
        /// <returns>A reference to the transposed matrix.</returns>
        mat3& Transpose()
        {
            *this = glm::transpose(*this);
            return *this;
        }

        /// <summary>
        /// Calculate the determinant of the matrix.
        /// </summary>
        /// <returns>The determinant of the matrix.</returns>
        T Determinant() const
        {
            return glm::determinant(*this);
        }

        /// <summary>
        /// Converts a 3x3 rotation matrix to Euler angles (in radians).
        /// </summary>
        /// <param name="matrix">3x3 rotation matrix.</param>
        /// <returns>Euler angles in radians.</returns>
        vec3 ToEulerAngles(const mat3& matrix) const
        {
            vec3 out;
            const float a = asinf(-matrix[0][2]);
            const float ca = cos(a);

            if (fabsf(ca) > 0.005f)
            {
                out[0] = atan2f(matrix[1][2] / ca, matrix[2][2] / ca);
                out[1] = a;
                out[2] = atan2f(matrix[0][1] / ca, matrix[0][0] / ca);
            }
            else
            {
                out[0] = atan2f(-matrix[2][1] / ca, matrix[1][1] / ca);
                out[1] = a;
                out[2] = 0.0f;
            }
            return out;
        }

        /// <summary>
        /// Converts a 3x3 rotation matrix to Euler angles (in degrees).
        /// </summary>
        /// <param name="matrix">3x3 rotation matrix.</param>
        /// <returns>Euler angles in degrees.</returns>
        vec3 ToEulerAnglesDeg(const mat3& matrix) const
        {
            vec3 eulerRad = ToEulerAngles(matrix);
            return { RadToDeg(eulerRad[0]), RadToDeg(eulerRad[1]), RadToDeg(eulerRad[2]) };
        }

        /// <summary>
        /// Converts Euler angles to an axis of rotation.
        /// </summary>
        /// <param name="angles">Euler angles in radians.</param>
        mat3& AnglesToAxis(const vec3& angles)
        {
            vec3 right;
            AngleVectors(angles, (*this)[0], right, (*this)[2]);

            (*this)[1][0] = -right[0];
            (*this)[1][1] = -right[1];
            (*this)[1][2] = -right[2];
            return *this;
        }

        /// <summary>
        /// Creates a 3x3 rotation matrix from Euler angles in radians.
        /// </summary>
        /// <param name="angles">Euler angles in radians.</param>
        mat3& CreateRotationMatrix(const vec3& angles)
        {
            AngleVectors(angles, (*this)[0], (*this)[1], (*this)[2]);

            (*this)[1][0] = -(*this)[1][0];
            (*this)[1][1] = -(*this)[1][1];
            (*this)[1][2] = -(*this)[1][2];
            return *this;
        }

        /// <summary>
        /// Rotates a 3D point using a 3x3 rotation matrix.
        /// </summary>
        /// <param name="point">Input point.</param>
        /// <returns>Rotated point.</returns>
        vec3 RotatePoint(const vec3& point)
        {
            point[0] = (*this)[0].Dot(point);
            point[1] = (*this)[1].Dot(point);
            point[2] = (*this)[2].Dot(point);
            return point;
        }

        /// <summary>
        /// Converts a unit quaternion to a 3x3 rotation matrix.
        /// </summary>
        /// <param name="quat">Input unit quaternion (x, y, z, w).</param>
        mat3& UnitQuatToAxis(const vec4& quat)
        {
            float xx, xy, xz, xw;
            float yy, yz, yw;
            float zz, zw;

            const float scaledX = quat[0] + quat[0];
            xx = scaledX * quat[0];
            xy = scaledX * quat[1];
            xz = scaledX * quat[2];
            xw = scaledX * quat[3];

            const float scaledY = quat[1] + quat[1];
            yy = scaledY * quat[1];
            yz = scaledY * quat[2];
            yw = scaledY * quat[3];

            const float scaledZ = quat[2] + quat[2];
            zz = scaledZ * quat[2];
            zw = scaledZ * quat[3];

            (*this)[0] = 1.0f - (yy + zz);
            (*this)[1] = xy + zw;
            (*this)[2] = xz - yw;
            (*this)[3] = xy - zw;
            (*this)[4] = 1.0f - (xx + zz);
            (*this)[5] = yz + xw;
            (*this)[6] = xz + yw;
            (*this)[7] = yz - xw;
            (*this)[8] = 1.0f - (xx + yy);
            return *this;
        }

        /// <summary>
        /// Convert the matrix to raw data.
        /// </summary>
        /// <returns>Pointer to raw data.</returns>
        operator T*() const
        {
            return reinterpret_cast<T*>(const_cast<mat3*>(this));
        }
    };
}
