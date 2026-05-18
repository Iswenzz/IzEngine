#pragma once
#include "Base.hpp"
#include "GraphicsContext.hpp"

#include "Engine/Renderer/Base/RendererAPI.hpp"

namespace IzEngine
{
	class DX9RendererAPI : public RendererAPI
	{
	public:
		~DX9RendererAPI() override = default;

		void Initialize() override;
		void Shutdown() override;

		void Begin() override;
		void End() override;
		void Resize(const vec2& size) override;

		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		void Clear() override;

		void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
		void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override;
		void SetLineWidth(float width) override;

		void* GetDevice() override;

	private:
		static inline Scope<DX9GraphicsContext> Context = nullptr;
	};
}
