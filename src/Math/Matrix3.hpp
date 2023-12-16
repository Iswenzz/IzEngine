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
        static Matrix3<float> Zero;
        static Matrix3<float> Identity;

        /// <summary>
        /// Initialize a 3x3 matrix.
        /// </summary>
        Matrix3() : gmat3<T>(0) { }

        /// <summary>
        /// Initialize a 3x3 matrix.
        /// </summary>
        /// <param name="v">The value.</param>
        Matrix3(T v) : gmat3<T>(v) { }

        /// <summary>
        /// Initialize a 3x3 matrix with specified values.
        /// </summary>
        /// <param name="m">An array of 9 values.</param>
        Matrix3(T x0, T y0, T z0, T x1, T y1, T z1, T x2, T y2, T z2) 
            : gmat3<T>(x0, y0, z0, x1, y1, z1, x2, y2, z2) { }

        /// <summary>
        /// Initialize a 3x3 matrix with specified values.
        /// </summary>
        /// <param name="m">An array of 9 values.</param>
        Matrix3(T* m) : gmat3<T>(m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8]) { }

        /// <summary>
        /// Initialize a 3x3 matrix with specified values.
        /// </summary>
        /// <param name="m">A 2D array of 3x3 values.</param>
        Matrix3(T m[3][3]) : gmat3<T>(vec3{ m[0] }, vec3{ m[1] }, vec3{ m[2] }) { }

        /// <summary>
        /// Initialize a 3x3 matrix with specified values.
        /// </summary>
        /// <param name="x">Row 1.</param>
        /// <param name="y">Row 2.</param>
        /// <param name="z">Row 3.</param>
        Matrix3(vec3 x, vec3 y, vec3 z) : gmat3<T>(x, y, z) { }

        /// <summary>
        /// Initialize a 3x3 matrix with specified values.
        /// </summary>
        /// <param name="m">The matrix.</param>
        Matrix3(gmat3<T> m) : gmat3<T>(m) { }

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
        vec3 RotatePoint(const vec3& point) const
        {
            vec3 rotate;
            rotate[0] = (*this)[0].Dot(point);
            rotate[1] = (*this)[1].Dot(point);
            rotate[2] = (*this)[2].Dot(point);
            return rotate;
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

            (*this)[0][0] = 1.0f - (yy + zz);
            (*this)[0][1] = xy + zw;
            (*this)[0][2] = xz - yw;
            (*this)[1][0] = xy - zw;
            (*this)[1][1] = 1.0f - (xx + zz);
            (*this)[1][2] = yz + xw;
            (*this)[2][0] = xz + yw;
            (*this)[2][1] = yz - xw;
            (*this)[2][2] = 1.0f - (xx + yy);
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

        /// <summary>
        /// Matrix is not zero.
        /// </summary>
        operator bool() const
        {
            return *this != Zero;
        }

        /// <summary>
        /// Matrix is zero.
        /// </summary>
        /// <returns></returns>
        bool operator!() const
        {
            return !operator bool();
        }

        NLOHMANN_SERIALIZE(mat3, value)
    };

    mat3f mat3f::Zero = (0, 0, 0, 0, 0, 0, 0, 0, 0);
    mat3f mat3f::Identity = (1, 0, 0, 0, 1, 0, 0, 0, 1);
}
