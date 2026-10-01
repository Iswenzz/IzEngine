#pragma once
#include "Base.hpp"

namespace IzEngine
{
	// A swapchain format in a graphics API's own values, and whether pixels in BGRA order need their red
	// and blue swapped to fit it.
	struct XRFormat
	{
		int64_t Format = 0;
		bool Swizzle = false;
	};

	// What OpenXR needs from one graphics API: the extension that binds it, a device on the headset's GPU,
	// and the images behind the swapchains. One per API, as ImGui has one backend per API.
	class API XRGraphics
	{
	public:
		virtual ~XRGraphics() = default;

		virtual const char* Extension() const = 0;
		virtual bool Initialize(XrInstance instance, XrSystemId system, std::string& error) = 0;
		virtual void Shutdown() = 0;
		virtual const void* Binding() const = 0;
		virtual std::span<const XRFormat> Formats() const = 0;
		virtual bool Images(XrSwapchain swapchain, std::vector<void*>& images) = 0;
	};
}
