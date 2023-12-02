#pragma once
#include "GLM.hpp"
#include <nlohmann/json.hpp>

namespace IW3SR
{
	/// <summary>
	/// Vector 3D class.
	/// </summary>
	template <typename T>
	class Vector3 : public gvec3<T>
	{
		using vec3 = Vector3<T>;

	public:
		static Vector3<float> Zero;
		static Vector3<float> One;
		static Vector3<float> UnitX;
		static Vector3<float> UnitY;
		static Vector3<float> UnitZ;
		static Vector3<float> Up;
		static Vector3<float> Down;
		static Vector3<float> Left;
		static Vector3<float> Right;

		/// <summary>
		/// Initialize vector.
		/// </summary>
		Vector3() : gvec3<T>(0) { }
		~Vector3() = default;

		/// <summary>
		/// Initialize the vector.
		/// </summary>
		/// <param name="v">The initial value.</param>
		Vector3(T v) : gvec3<T>(v) { }

		/// <summary>
		/// Initialize vector.
		/// </summary>
		/// <param name="x">X value.</param>
		/// <param name="y">Y value.</param>
		/// <param name="z">Z value.</param>
		Vector3(T x, T y, T z) : gvec3<T>(x, y, z) { }

		/// <summary>
		/// Initilize vector.
		/// </summary>
		/// <param name="v">The values.</param>
		Vector3(T* v) : gvec3<T>(v[0], v[1], v[2]) { }

		/// <summary>
		/// Initialize the vector.
		/// </summary>
		/// <param name="v">The initial value.</param>
		Vector3(gvec3<T> v) : gvec3<T>(v) { }

		/// <summary>
		/// Normalize the vector.
		/// </summary>
		/// <returns>A reference to the normalized vector.</returns>
		vec3& Normalize()
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
		vec3& Normalize(T& lenOut)
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
		vec3& Lerp(const vec3& start, const vec3& end, const float fract)
		{
			if (fract == 1.0f)
				return *this;

			*this = start + fract * (end - start);
			return *this;
		}

		/// <summary>
		/// Calculate the cross product of this vector and another vector.
		/// </summary>
		/// <param name="v">The other vector.</param>
		/// <returns>A reference to the resulting cross product.</returns>
		vec3& Cross(const vec3& v)
		{
			const vec3 og = *this;

			this->x = og.y * v.z - og.z * v.y;
			this->y = og.z * v.x - og.x * v.z;
			this->z = og.x * v.y - og.y * v.x;

			return *this;
		}

		/// <summary>
		/// Calculate the distance between this vector and another vector.
		/// </summary>
		/// <param name="v">The other vector.</param>
		/// <returns>The distance between the vectors.</returns>
		T Distance(const vec3& v) const
		{
			const vec3 d(v.x - this->x, v.y - this->y, v.z - this->z);
			return d.Length();
		}

		/// <summary>
		/// Calculate the squared length of this vector.
		/// </summary>
		/// <returns>The squared length of the vector.</returns>
		T LengthSquared() const
		{
			return this->x * this->x + this->y * this->y + this->z * this->z;
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
		/// <returns>The dot product of the vectors.</returns>
		T Dot(const vec3& v) const
		{
			return this->x * v.x + this->y * v.y + this->z * v.z;
		}

		/// <summary>
		/// Compares two vec3 vectors element-wise within a specified epsilon range.
		/// </summary>
		/// <param name="v">Vector to compare.</param>
		/// <param name="epsilon">Epsilon value for tolerance in comparisons.</param>
		/// <returns>True if all elements of the vectors are within epsilon range, otherwise false.</returns>
		bool NearEqual(const vec3& v, float epsilon) const
		{
			return (((*this)[0] - epsilon <= v[0] && (*this)[0] + epsilon >= v[0])
				&& ((*this)[1] - epsilon <= v[1] && (*this)[1] + epsilon >= v[1])
				&& ((*this)[2] - epsilon <= v[2] && (*this)[2] + epsilon >= v[2]));
		}

		/// <summary>
		/// Convert the vector to raw data.
		/// </summary>
		/// <returns>Pointer to raw data.</returns>
		operator T*() const
		{
			return reinterpret_cast<T*>(const_cast<vec3*>(this));
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

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(vec3, x, y, z)
	};

	vec3f vec3f::Zero = (0, 0, 0);
	vec3f vec3f::One = (1, 1, 1);
	vec3f vec3f::UnitX = (1, 0, 0);
	vec3f vec3f::UnitY = (0, 1, 0);
	vec3f vec3f::UnitZ = (0, 0, 1);
	vec3f vec3f::Up = (0, 1, 0);
	vec3f vec3f::Down = (0, -1, 0);
	vec3f vec3f::Left = (-1, 0, 0);
	vec3f vec3f::Right = (1, 0, 0);
}
