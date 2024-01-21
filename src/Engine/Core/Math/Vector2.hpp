#pragma once
#include "Engine/Core/Math.hpp"
#include "Engine/Backends/ImGUI.hpp"
#include "Engine/Backends/DX9.hpp"

namespace IW3SR::Engine
{
    /// <summary>
    /// Vector 2D class.
    /// </summary>
    template <typename T>
    class Vector2 : public gvec2<T>
    {
        using vec2 = Vector2<T>;

    public:
        static Vector2<float> Zero;
        static Vector2<float> One;
        static Vector2<float> UnitX;
        static Vector2<float> UnitY;

        /// <summary>
        /// Initialize vector.
        /// </summary>
        Vector2() : gvec2<T>(0) { }

        /// <summary>
        /// Initialize the vector.
        /// </summary>
        /// <param name="v">The initial value.</param>
        Vector2(T v) : gvec2<T>(v) { }

        /// <summary>
        /// Initialize vector.
        /// </summary>
        /// <param name="x">X value.</param>
        /// <param name="y">Y value.</param>
        Vector2(T x, T y) : gvec2<T>(x, y) { }

        /// <summary>
        /// Initilize vector.
        /// </summary>
        /// <param name="v">The values.</param>
        Vector2(T* v) : gvec2<T>(v[0], v[1]) { }

        /// <summary>
        /// Initialize the vector.
        /// </summary>
        /// <param name="v">The initial value.</param>
        Vector2(gvec2<T> v) : gvec2<T>(v) { }

        /// <summary>
        /// Initialize the vector.
        /// </summary>
        /// <param name="v">The initial value.</param>
        Vector2(ImVec2 v) : gvec2<T>(v[0], v[1]) { }

        /// <summary>
        /// Normalize the vector.
        /// </summary>
        /// <returns>A reference to the normalized vector.</returns>
        vec2& Normalize()
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
        vec2& Normalize(T& lenOut)
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
        vec2& Lerp(const vec2& start, const vec2& end, const float fract)
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
        T Distance(const vec2& v) const
        {
            const vec2 d(v.x - this->x, v.y - this->y);
            return d.Length();
        }

        /// <summary>
        /// Calculate the squared length of this vector.
        /// </summary>
        /// <returns>The squared length of the vector.</returns>
        T LengthSquared() const
        {
            return this->x * this->x + this->y * this->y;
        }

        /// <summary>
        /// Calculate the length of this vector.
        /// </summary>
        /// <returns>The length of the vector.</returns>
        T Length() const
        {
            return std::sqrt(LengthSquared());
        }

        /// <summary>
        /// Calculate the dot product.
        /// </summary>
        /// <param name="v">The other vector.</param>
        /// <param name="v2">The second vector.</param>
        /// <returns>The dot product of the vectors.</returns>
        T Dot(const vec2& v) const
        {
            return this->x * v.x + this->y * v.y;
        }

        /// <summary>
        /// Compares two vec2 vectors element-wise within a specified epsilon range.
        /// </summary>
        /// <param name="v">Vector to compare.</param>
        /// <param name="epsilon">Epsilon value for tolerance in comparisons.</param>
        /// <returns>True if all elements of the vectors are within epsilon range, otherwise false.</returns>
        bool NearEqual(const vec2& v, float epsilon) const
        {
            return (((*this)[0] - epsilon <= v[0] && (*this)[0] + epsilon >= v[0])
                && ((*this)[1] - epsilon <= v[1] && (*this)[1] + epsilon >= v[1]));
        }

        /// <summary>
        /// Convert the vector to raw data.
        /// </summary>
        /// <returns>Pointer to raw data.</returns>
        operator T*() const
        {
            return reinterpret_cast<T*>(const_cast<vec2*>(this));
        }

        /// <summary>
        /// Convert to imgui vector.
        /// </summary>
        operator ImVec2() const
        {
            return *reinterpret_cast<const ImVec2*>(this);
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

        NLOHMANN_SERIALIZE(vec2, x, y)
    };

    vec2f vec2f::Zero = (0, 0);
    vec2f vec2f::One = (1, 1);
    vec2f vec2f::UnitX = (1, 0);
    vec2f vec2f::UnitY = (0, 1);
}
