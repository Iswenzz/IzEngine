#pragma once
#include "Base.hpp"

#include "Engine/Renderer/Base/Shader.hpp"

namespace IzEngine
{
	class DX9Shader : public Shader
	{
	public:
		DX9Shader() = default;
		~DX9Shader() override;

		void Bind() const override;
		void Unbind() const override;
		void Release() override;
		void OnBeforeReset() override;
		void OnAfterReset() override;

		void SetInt(const std::string& name, int value) override;
		void SetIntArray(const std::string& name, int* values, uint32_t count) override;
		void SetFloat(const std::string& name, float value) override;
		void SetFloat2(const std::string& name, const vec2& value) override;
		void SetFloat3(const std::string& name, const vec3& value) override;
		void SetFloat4(const std::string& name, const vec4& value) override;
		void SetMat3(const std::string& name, const mat3& value) override;
		void SetMat4(const std::string& name, const mat4& value) override;

		static Ref<Shader> Create(const ShaderSpecification& spec);

	private:
		IDirect3DVertexShader9* VertexShader = nullptr;
		IDirect3DPixelShader9* PixelShader = nullptr;
		ID3DXConstantTable* VertexConstants = nullptr;
		ID3DXConstantTable* PixelConstants = nullptr;

		void Compile();
	};
}
