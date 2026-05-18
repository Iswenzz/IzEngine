#pragma once
#include "Engine/Core/Common.hpp"
#include "Engine/Renderer/Camera/Camera.hpp"

namespace IzEngine
{
	class Renderer
	{
	public:
		static inline bool Active = false;
		static inline Camera Camera2D;
		static inline Camera Camera3D;

		static void Initialize(RendererBackend api);
		static void Shutdown();

		static void Resize(const vec2& size);
		static void Begin();
		static void End();

		static void Frame();
	};
}
