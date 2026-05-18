#pragma once
#include "Base.hpp"

#include "Engine/Renderer/Resources/Texture.hpp"

namespace IzEngine
{
	class DX9Texture : public Texture
	{
	public:
		IDirect3DTexture9* Data = nullptr;
		IDirect3DSurface9* Surface = nullptr;

		DX9Texture() = default;
		~DX9Texture() override;

		void Bind(uint32_t slot) const override;
		void Unbind(uint32_t slot) const override;
		void Release() override;
		void OnDeviceLost() override;
		void OnDeviceReset() override;

		vec2 GetSize() const override;

		static Ref<Texture> Create(const TextureSpecification& spec);
	};
}
