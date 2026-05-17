#pragma once
#include "Core/Common.hpp"

namespace IzEngine
{
	class Renderer
	{
	public:
		static inline bool Active = false;

		static void Initialize();
		static void InitializeAssets();
		static void Shutdown();
		static void ShutdownAssets();
		static void DeviceLost();
		static void DeviceReset();

		static void Resize(const vec2& size);
		static void Begin();
		static void End();

		static void Frame();
	};
}
