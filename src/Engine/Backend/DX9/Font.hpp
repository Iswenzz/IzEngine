#pragma once
#include "Base.hpp"

#include "Engine/Renderer/Resources/Font.hpp"

namespace IzEngine
{
	class DX9Font : public Font
	{
	public:
		ID3DXFont* Data = nullptr;

		DX9Font() = default;
		~DX9Font() override;

		void Release() override;
		void OnDeviceLost() override;
		void OnDeviceReset() override;

		vec2 GetTextSize(const std::string& text) const override;

		static Ref<Font> Create(const FontSpecification& spec);
	};
}
