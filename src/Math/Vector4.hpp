#pragma once
#include "GLM.hpp"
#include "Game/Render/ImGUI.hpp"

#include <nlohmann/json.hpp>

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
        static Vector4<float> Zero;
        static Vector4<float> One;
        static Vector4<float> X;
        static Vector4<float> Y;
        static Vector4<float> Z;
        static Vector4<float> W;

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
        /// Initialize the vector.
        /// </summary>
        /// <param name="v">The initial value.</param>
        Vector4(ImVec4 v) : gvec4<T>(v[0], v[1], v[2], v[3]) { }

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
        /// <param name="v">Vector to compare.</param>
        /// <param name="epsilon">Epsilon value for tolerance in comparisons.</param>
        /// <returns>True if all elements of the vectors are within epsilon range, otherwise false.</returns>
        bool NearEqual(const vec4& v, float epsilon) const
        {
            return (((*this)[0] - epsilon <= v[0] && (*this)[0] + epsilon >= v[0])
                && ((*this)[1] - epsilon <= v[1] && (*this)[1] + epsilon >= v[1])
                && ((*this)[2] - epsilon <= v[2] && (*this)[2] + epsilon >= v[2])
                && ((*this)[3] - epsilon <= v[3] && (*this)[3] + epsilon >= v[3]));
        }

        /// <summary>
        /// Convert the vector to raw data.
        /// </summary>
        /// <returns>Pointer to raw data.</returns>
        operator T*() const
        {
            return reinterpret_cast<T*>(const_cast<vec4*>(this));
        }

        /// <summary>
        /// Convert to imgui vector.
        /// </summary>
        operator ImVec4() const
        {
            return *reinterpret_cast<const ImVec4*>(this);
        }

        /// <summary>
        /// Convert to imgui data.
        /// </summary>
        operator ImU32() const
        {
            ImU32 u32 = 0;
            u32 |= (static_cast<ImU32>(this->x * 255) & 0xFF) << 0;
            u32 |= (static_cast<ImU32>(this->y * 255) & 0xFF) << 8;
            u32 |= (static_cast<ImU32>(this->z * 255) & 0xFF) << 16;
            u32 |= (static_cast<ImU32>(this->w * 255) & 0xFF) << 24;
            return u32;
        }

        /// <summary>
        /// Vector is not zero.
        /// </summary>
        operator bool() const
        {
            return *this != Zero;
        }

        /// <summary>
        /// Vector is zero.
        /// </summary>
        /// <returns></returns>
        bool operator!() const
        {
            return !operator bool();
        }

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(vec4, x, y, z, w);
    };

    vec4f vec4f::Zero = (0, 0, 0, 0);
    vec4f vec4f::One = (1, 1, 1, 1);
    vec4f vec4f::X = (1, 0, 0, 0);
    vec4f vec4f::Y = (0, 1, 0, 0);
    vec4f vec4f::Z = (0, 0, 1, 0);
    vec4f vec4f::W = (0, 0, 0, 1);
}
