#pragma once
#include "GLM.hpp"

namespace IW3SR
{
    /// <summary>
    /// Vector 4D class.
    /// </summary>
    template <typename T>
    class Vector4 : public gvec4<T>
    {
        using vec4 = Vector4<T>;

    public:
        /// <summary>
        /// Initialize vector.
        /// </summary>
        Vector4() : gvec4<T>(0) { }
        ~Vector4() = default;

        /// <summary>
        /// Initialize the vector.
        /// </summary>
        /// <param name="v">The initial value.</param>
        Vector4(T v) : gvec4<T>(v) { }

        /// <summary>
        /// Initialize vector.
        /// </summary>
        /// <param name="x">X value.</param>
        /// <param name="y">Y value.</param>
        /// <param name="z">Z value.</param>
        /// <param name="w">W value.</param>
        Vector4(T x, T y, T z, T w) : gvec4<T>(x, y, z, w) { }

        /// <summary>
        /// Initilize vector.
        /// </summary>
        /// <param name="v">The values.</param>
        Vector4(T* v) : gvec4<T>(v[0], v[1], v[2], v[3]) { }

        /// <summary>
        /// Initilize vector.
        /// </summary>
        /// <param name="v">The values.</param>
        Vector4(gvec4<T> v) : gvec4<T>(v) { }

        /// <summary>
        /// Normalize the vector.
        /// </summary>
        /// <returns>A reference to the normalized vector.</returns>
        vec4& Normalize()
        {
            const float len = Length();
            if (len == 0.0f)
                return *this;

            *this *= (1.0f / len);
            return *this;
        }

        /// <summary>
        /// Normalize the vector and return its original length.
        /// </summary>
        /// <param name="lenOut">Length of the original vector.</param>
        /// <returns>A reference to the normalized vector.</returns>
        vec4& Normalize(T& lenOut)
        {
            lenOut = Length();
            if (lenOut == 0.0f)
                return *this;

            *this *= (1.0f / lenOut);
            return *this;
        }

        /// <summary>
        /// Perform linear interpolation between two vectors.
        /// </summary>
        /// <param name="start">The start vector.</param>
        /// <param name="end">The end vector.</param>
        /// <param name="fract">Interpolation fraction.</param>
        /// <returns>The result of the linear interpolation.</returns>
        vec4& Lerp(const vec4& start, const vec4& end, const float fract)
        {
            if (fract == 1.0f)
                return *this;

            *this = start + fract * (end - start);
            return *this;
        }

        /// <summary>
        /// Calculate the distance between this vector and another vector.
        /// </summary>
        /// <param name="v">The other vector.</param>
        /// <returns>The distance between the vectors.</returns>
        T Distance(const vec4& v) const
        {
            const vec4 d(v.x - this->x, v.y - this->y, v.z - this->z, v.w - this->w);
            return d.Length();
        }

        /// <summary>
        /// Calculate the squared length of this vector.
        /// </summary>
        /// <returns>The squared length of the vector.</returns>
        T LengthSquared() const
        {
            return this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w;
        }

        /// <summary>
        /// Calculate the length of this vector.
        /// </summary>
        /// <returns>The length of the vector.</returns>
        T Length() const
        {
            return std::sqrtf(LengthSquared());
        }

        /// <summary>
        /// Calculate the dot product.
        /// </summary>
        /// <param name="v">The other vector.</param>
        /// <returns>The dot product of the vectors.</returns>
        T Dot(const vec4& v) const
        {
            return this->x * v.x + this->y * v.y + this->z * v.z + this->w * v.w;
        }

        /// <summary>
        /// Compares two vec4 vectors element-wise within a specified epsilon range.
        /// </summary>
        /// <param name="v1">First vec4 vector to compare.</param>
        /// <param name="v2">Second vec4 vector to compare.</param>
        /// <param name="epsilon">Epsilon value for tolerance in comparisons.</param>
        /// <returns>True if all elements of the vectors are within epsilon range, otherwise false.</returns>
        bool NearEqual(const vec4& v1, const vec4& v2, float epsilon) const
        {
            return ((v1[0] - epsilon <= v2[0] && v1[0] + epsilon >= v2[0])
                && (v1[1] - epsilon <= v2[1] && v1[1] + epsilon >= v2[1])
                && (v1[2] - epsilon <= v2[2] && v1[2] + epsilon >= v2[2])
                && (v1[3] - epsilon <= v2[3] && v1[3] + epsilon >= v2[3]));
        }

        /// <summary>
        /// Convert the vector to raw data.
        /// </summary>
        /// <returns>Pointer to raw data.</returns>
        operator T*() const
        {
            return reinterpret_cast<T*>(const_cast<vec4*>(this));
        }
    };
}
