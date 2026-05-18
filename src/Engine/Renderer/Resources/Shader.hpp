#pragma once
#include "Engine/Core/IO/File.hpp"
#include "Engine/Renderer/Base/GPUResource.hpp"

namespace IzEngine
{
	struct ShaderSpecification
	{
		std::string ID;
		File VertexSource;
		File PixelSource;
	};

	class API Shader : public GPUResource
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Release() = 0;

		virtual const std::string& GetName() const = 0;

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat2(const std::string& name, const vec2& value) = 0;
		virtual void SetFloat3(const std::string& name, const vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const vec4& value) = 0;
		virtual void SetMat3(const std::string& name, const mat3& value) = 0;
		virtual void SetMat4(const std::string& name, const mat4& value) = 0;

		static Ref<Shader> Create(const ShaderSpecification& spec);
	};
}
