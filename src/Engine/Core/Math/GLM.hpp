#pragma once
#include "Engine/Base.hpp"
#include "ImGUI/Base.hpp"

#define GLM_FORCE_XYZW_ONLY
#define GLM_FORCE_LEFT_HANDED

#include <glm/glm.hpp>

namespace IzEngine
{
	template <typename T>
	class Matrix3;
	template <typename T>
	class Vector2;
	template <typename T>
	class Vector3;
	template <typename T>
	class Vector4;

	template <typename T>
	using gmat3 = glm::mat<3, 3, T, glm::packed_highp>;
	template <typename T>
	using gvec2 = glm::vec<2, T, glm::packed_highp>;
	template <typename T>
	using gvec3 = glm::vec<3, T, glm::packed_highp>;
	template <typename T>
	using gvec4 = glm::vec<4, T, glm::packed_highp>;

	using mat3c = Matrix3<char>;
	using mat3s = Matrix3<short>;
	using mat3i = Matrix3<int>;
	using mat3f = Matrix3<float>;
	using mat3d = Matrix3<double>;
	using mat3 = mat3f;

	using mat3x3c = mat3c;
	using mat3x3s = mat3s;
	using mat3x3i = mat3i;
	using mat3x3f = mat3f;
	using mat3x3d = mat3d;
	using mat3x3 = mat3f;

	using vec2c = Vector2<char>;
	using vec2s = Vector2<short>;
	using vec2i = Vector2<int>;
	using vec2f = Vector2<float>;
	using vec2d = Vector2<double>;
	using vec2 = vec2f;

	using vec3c = Vector3<char>;
	using vec3s = Vector3<short>;
	using vec3i = Vector3<int>;
	using vec3f = Vector3<float>;
	using vec3d = Vector3<double>;
	using vec3 = vec3f;

	using vec4c = Vector4<char>;
	using vec4s = Vector4<short>;
	using vec4i = Vector4<int>;
	using vec4f = Vector4<float>;
	using vec4d = Vector4<double>;
	using vec4 = vec4f;
}
