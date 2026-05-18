#include "RenderCommand.hpp"

namespace IzEngine
{
	void RenderCommand::Initialize(RendererBackend api)
	{
		RendererAPI = RendererAPI::Create(api);
		RendererAPI->Initialize();
	}

	void RenderCommand::Shutdown()
	{
		RendererAPI->Shutdown();
		RendererAPI = nullptr;
	}

	void RenderCommand::Begin()
	{
		RendererAPI->Begin();
	}

	void RenderCommand::End()
	{
		RendererAPI->End();
	}

	void RenderCommand::Resize(const vec2& size)
	{
		RendererAPI->Resize(size);
	}

	void RenderCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		RendererAPI->SetViewport(x, y, width, height);
	}

	void RenderCommand::Clear()
	{
		RendererAPI->Clear();
	}

	void RenderCommand::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		RendererAPI->DrawIndexed(vertexArray, indexCount);
	}

	void RenderCommand::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		RendererAPI->DrawLines(vertexArray, vertexCount);
	}

	void RenderCommand::SetLineWidth(float width)
	{
		RendererAPI->SetLineWidth(width);
	}

	void* RenderCommand::GetDevice()
	{
		return RendererAPI->GetDevice();
	}
}
