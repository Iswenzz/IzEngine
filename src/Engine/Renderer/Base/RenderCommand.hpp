#pragma once
#include "RendererAPI.hpp"

namespace IzEngine
{
	class RenderCommand
	{
	public:
		static void Initialize(RendererBackend api);
		static void Shutdown();

		static void Begin();
		static void End();
		static void Resize(const vec2& size);

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void Clear();

		static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0);
		static void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount);
		static void SetLineWidth(float width);

		static void* GetDevice();

	private:
		static inline Scope<RendererAPI> RendererAPI;
	};
}
