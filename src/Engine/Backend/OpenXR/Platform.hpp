#pragma once
#include "Graphics.hpp"

// openxr_platform.h is read once per file, so every graphics API a binding may need is declared before it,
// whichever binding includes it first.
#ifdef PLATFORM_WINDOWS
	#include <d3d11.h>
	#include <dxgi1_4.h>

	// d3d12.h names the system's UUID, which the engine's own would otherwise stand in for; it is a GUID.
	#pragma push_macro("UUID")
	#define UUID GUID
	#include <d3d12.h>
	#pragma pop_macro("UUID")

	#define XR_USE_GRAPHICS_API_D3D11
	#define XR_USE_GRAPHICS_API_D3D12
#endif

#ifdef BUILD_VULKAN
	// The engine links no Vulkan loader; functions come from the one the application's device runs on.
	#ifndef VK_NO_PROTOTYPES
		#define VK_NO_PROTOTYPES
	#endif
	#include <vulkan/vulkan.h>

	#define XR_USE_GRAPHICS_API_VULKAN
#endif

#include <openxr/openxr_platform.h>
