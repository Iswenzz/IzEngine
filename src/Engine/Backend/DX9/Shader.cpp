#include "Shader.hpp"
#include "GraphicsContext.hpp"

#include "Engine/Core/IO/File.hpp"
#include "Engine/Core/System/AssetManager.hpp"

namespace IzEngine
{
	static ID3DXConstantTable* CompileShader(const std::string& src, const std::string& target, IUnknown** shader)
	{
		ID3DBlob* bytecode = nullptr;
		ID3DBlob* errors = nullptr;
		ID3DXConstantTable* constants = nullptr;

		HRESULT hr = D3DCompile(src.c_str(), src.size(), nullptr, nullptr, nullptr, "main", target.c_str(),
			D3DCOMPILE_ENABLE_STRICTNESS, 0, &bytecode, &errors);

		if (FAILED(hr))
		{
			if (errors)
			{
				Log::WriteLine(Channel::Error, "Compilation failed: {}",
					reinterpret_cast<char*>(errors->GetBufferPointer()));
				errors->Release();
			}
			IZ_ASSERT(false, "Shader compilation failed.");
			return nullptr;
		}
		D3DXGetShaderConstantTable(static_cast<const DWORD*>(bytecode->GetBufferPointer()), &constants);

		if (target.find("vs") != std::string::npos)
		{
			DX9GraphicsContext::Device->CreateVertexShader(static_cast<const DWORD*>(bytecode->GetBufferPointer()),
				reinterpret_cast<IDirect3DVertexShader9**>(shader));
		}
		else
		{
			DX9GraphicsContext::Device->CreatePixelShader(static_cast<const DWORD*>(bytecode->GetBufferPointer()),
				reinterpret_cast<IDirect3DPixelShader9**>(shader));
		}
		bytecode->Release();
		return constants;
	}

	void DX9Shader::Compile(const std::string& vertexSrc, const std::string& pixelSrc)
	{
		VertexConstants = CompileShader(vertexSrc, "vs_3_0", reinterpret_cast<IUnknown**>(&VertexShader));
		PixelConstants = CompileShader(pixelSrc, "ps_3_0", reinterpret_cast<IUnknown**>(&PixelShader));
	}

	Ref<Shader> DX9Shader::Create(const ShaderSpecification& spec)
	{
		std::string id = spec.ID;

		IZ_ASSERT(!id.empty(), "ShaderSpecification requires an ID.");

		if (AssetManager::Exists(id))
			return AssetManager::Get<Shader>(id);

		IZ_ASSERT(DX9GraphicsContext::Device, "DX9Shader requires an initialized device.");
		IZ_ASSERT(spec.VertexSource.IsValid(), "Vertex shader source is invalid.");
		IZ_ASSERT(spec.PixelSource.IsValid(), "Pixel shader source is invalid.");

		Ref<DX9Shader> shader = CreateRef<DX9Shader>();
		shader->VertexSource = std::string(spec.VertexSource.Data.begin(), spec.VertexSource.Data.end());
		shader->PixelSource = std::string(spec.PixelSource.Data.begin(), spec.PixelSource.Data.end());
		shader->Compile(shader->VertexSource, shader->PixelSource);

		GPUResource::RegisterResource(shader.get());
		return AssetManager::Add(id, shader);
	}

	DX9Shader::~DX9Shader()
	{
		GPUResource::UnregisterResource(this);
		Release();
	}

	void DX9Shader::Release()
	{
		if (VertexShader)
		{
			VertexShader->Release();
			VertexShader = nullptr;
		}
		if (PixelShader)
		{
			PixelShader->Release();
			PixelShader = nullptr;
		}
		if (VertexConstants)
		{
			VertexConstants->Release();
			VertexConstants = nullptr;
		}
		if (PixelConstants)
		{
			PixelConstants->Release();
			PixelConstants = nullptr;
		}
	}

	void DX9Shader::Bind() const
	{
		IZ_ASSERT(VertexShader, "VertexShader is null.");
		IZ_ASSERT(PixelShader, "PixelShader is null.");

		DX9GraphicsContext::Device->SetVertexShader(VertexShader);
		DX9GraphicsContext::Device->SetPixelShader(PixelShader);
	}

	void DX9Shader::Unbind() const
	{
		DX9GraphicsContext::Device->SetVertexShader(nullptr);
		DX9GraphicsContext::Device->SetPixelShader(nullptr);
	}

	void DX9Shader::OnDeviceLost()
	{
		Release();
	}

	void DX9Shader::OnDeviceReset()
	{
		Compile(VertexSource, PixelSource);
	}

	const std::string& DX9Shader::GetName() const
	{
		return Name;
	}

	void DX9Shader::SetInt(const std::string& name, int value)
	{
		float v = static_cast<float>(value);
		if (VertexConstants)
			VertexConstants->SetFloat(DX9GraphicsContext::Device, name.c_str(), v);
		if (PixelConstants)
			PixelConstants->SetFloat(DX9GraphicsContext::Device, name.c_str(), v);
	}

	void DX9Shader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		std::vector<float> floats(count);
		for (uint32_t i = 0; i < count; i++)
			floats[i] = static_cast<float>(values[i]);

		if (VertexConstants)
			VertexConstants->SetFloatArray(DX9GraphicsContext::Device, name.c_str(), floats.data(), count);
		if (PixelConstants)
			PixelConstants->SetFloatArray(DX9GraphicsContext::Device, name.c_str(), floats.data(), count);
	}

	void DX9Shader::SetFloat(const std::string& name, float value)
	{
		if (VertexConstants)
			VertexConstants->SetFloat(DX9GraphicsContext::Device, name.c_str(), value);
		if (PixelConstants)
			PixelConstants->SetFloat(DX9GraphicsContext::Device, name.c_str(), value);
	}

	void DX9Shader::SetFloat2(const std::string& name, const vec2& value)
	{
		D3DXVECTOR4 v(value.x, value.y, 0.0f, 0.0f);
		if (VertexConstants)
			VertexConstants->SetVector(DX9GraphicsContext::Device, name.c_str(), &v);
		if (PixelConstants)
			PixelConstants->SetVector(DX9GraphicsContext::Device, name.c_str(), &v);
	}

	void DX9Shader::SetFloat3(const std::string& name, const vec3& value)
	{
		D3DXVECTOR4 v(value.x, value.y, value.z, 0.0f);
		if (VertexConstants)
			VertexConstants->SetVector(DX9GraphicsContext::Device, name.c_str(), &v);
		if (PixelConstants)
			PixelConstants->SetVector(DX9GraphicsContext::Device, name.c_str(), &v);
	}

	void DX9Shader::SetFloat4(const std::string& name, const vec4& value)
	{
		D3DXVECTOR4 v(value.x, value.y, value.z, value.w);
		if (VertexConstants)
			VertexConstants->SetVector(DX9GraphicsContext::Device, name.c_str(), &v);
		if (PixelConstants)
			PixelConstants->SetVector(DX9GraphicsContext::Device, name.c_str(), &v);
	}

	void DX9Shader::SetMat3(const std::string& name, const mat3& value)
	{
		D3DXMATRIX m(value[0][0], value[0][1], value[0][2], 0.0f, value[1][0], value[1][1], value[1][2], 0.0f,
			value[2][0], value[2][1], value[2][2], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		if (VertexConstants)
			VertexConstants->SetMatrix(DX9GraphicsContext::Device, name.c_str(), &m);
		if (PixelConstants)
			PixelConstants->SetMatrix(DX9GraphicsContext::Device, name.c_str(), &m);
	}

	void DX9Shader::SetMat4(const std::string& name, const mat4& value)
	{
		D3DXMATRIX m(value[0][0], value[0][1], value[0][2], value[0][3], value[1][0], value[1][1], value[1][2],
			value[1][3], value[2][0], value[2][1], value[2][2], value[2][3], value[3][0], value[3][1], value[3][2],
			value[3][3]);
		if (VertexConstants)
			VertexConstants->SetMatrix(DX9GraphicsContext::Device, name.c_str(), &m);
		if (PixelConstants)
			PixelConstants->SetMatrix(DX9GraphicsContext::Device, name.c_str(), &m);
	}
}
