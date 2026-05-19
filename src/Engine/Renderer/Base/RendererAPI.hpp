#pragma once
#include "Engine/Core/Common.hpp"

#include "VertexArray.hpp"

namespace IzEngine
{
	enum class RendererBackend
	{
		DX9
	};

	class RendererAPI
	{
	public:
		virtual ~RendererAPI() = default;

		virtual void Initialize() = 0;
		virtual void Shutdown() = 0;

		virtual void Begin() = 0;
		virtual void End() = 0;
		virtual void Resize(const vec2& size) = 0;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) = 0;
		virtual void SetLineWidth(float width) = 0;

		virtual void* GetDevice() = 0;

		static Scope<RendererAPI> Create(RendererBackend api);
		static RendererBackend GetAPI();
		static std::string GetShaderPath(const std::string& name);

	private:
		static inline RendererBackend Backend;
	};
}
