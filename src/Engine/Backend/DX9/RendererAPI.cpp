#include "RendererAPI.hpp"
#include "GraphicsContext.hpp"

#include "Engine/Backend/ImGUI/Common.hpp"
#include "Engine/Core/System/Window.hpp"
#include "Engine/Renderer/Base/VertexArray.hpp"

namespace IzEngine
{
	void DX9RendererAPI::Initialize()
	{
		IZ_ASSERT(Window::Handle, "Window is not initialized.");

		DX9GraphicsContext::Initialize();

		ImGui_ImplOS_Init(Window::Handle);
		ImGui_ImplAPI_Init(DX9GraphicsContext::Device);
	}

	void DX9RendererAPI::Shutdown()
	{
		ImGui_ImplOS_Shutdown();
		ImGui_ImplAPI_Shutdown();

		DX9GraphicsContext::Shutdown();
	}

	void DX9RendererAPI::Begin()
	{
		if (DX9GraphicsContext::Swapped)
		{
			DX9GraphicsContext::SaveState();
		}
		else
		{
			DX9GraphicsContext::Device->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_COLORVALUE(0, 0, 0, 1), 1.0f, 0);
			DX9GraphicsContext::Device->BeginScene();
		}
		ImGui_ImplOS_NewFrame();
		ImGui_ImplAPI_NewFrame();
	}

	void DX9RendererAPI::End()
	{
		ImGui_ImplAPI_RenderDrawData(ImGui::GetDrawData());

		if (DX9GraphicsContext::Swapped)
		{
			DX9GraphicsContext::RestoreState();
		}
		else
		{
			DX9GraphicsContext::Device->EndScene();
			DX9GraphicsContext::Present();
		}
	}

	void DX9RendererAPI::Resize(const vec2& size)
	{
		DX9GraphicsContext::Resize(size);
	}

	void DX9RendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		D3DVIEWPORT9 viewport;
		viewport.X = x;
		viewport.Y = y;
		viewport.Width = width;
		viewport.Height = height;
		viewport.MinZ = 0.0f;
		viewport.MaxZ = 1.0f;

		DX9GraphicsContext::Device->SetViewport(&viewport);
	}

	void DX9RendererAPI::Clear()
	{
		DX9GraphicsContext::Device->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_COLORVALUE(0, 0, 0, 1), 1.0f, 0);
	}

	void DX9RendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		IZ_ASSERT(vertexArray, "VertexArray is null.");

		vertexArray->Bind();
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		DX9GraphicsContext::Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, count, 0, count / 3);
	}

	void DX9RendererAPI::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		IZ_ASSERT(vertexArray, "VertexArray is null.");

		vertexArray->Bind();
		DX9GraphicsContext::Device->DrawPrimitive(D3DPT_LINELIST, 0, vertexCount / 2);
	}

	void DX9RendererAPI::SetLineWidth(float width) { }

	void* DX9RendererAPI::GetDevice()
	{
		return DX9GraphicsContext::Device;
	}
}
