#include "Renderer.hpp"

#include "Engine/Core/System/AssetManager.hpp"
#include "Engine/Core/System/Window.hpp"

namespace IzEngine
{
	void Renderer::Initialize(RendererBackend api)
	{
		IZ_ASSERT(!Active, "Renderer already initialized.");
		IZ_ASSERT(Window::Handle, "Window is not initialized.");

		Camera2D = Camera::CreateOrthographic(Window::Size.x, Window::Size.y);
		Camera3D = Camera::CreatePerspective(65.0f, Window::Size.x / Window::Size.y, 0.1f, 1000.0f);

		UI::Initialize();
		RenderCommand::Initialize(api);
		AssetManager::Initialize();
		Draw2D::Initialize();

		Active = true;
	}

	void Renderer::Shutdown()
	{
		IZ_ASSERT(Active, "Renderer already shutdown.");

		Active = false;

		Draw2D::Shutdown();
		AssetManager::Shutdown();
		RenderCommand::Shutdown();
		UI::Shutdown();
	}

	void Renderer::Resize(const vec2& size)
	{
		Camera2D.Resize(size.x, size.y);
		Camera3D.Resize(size.x, size.y);

		RenderCommand::Resize(size);
		UI::Resize(size);
	}

	void Renderer::Begin()
	{
		RenderCommand::Begin();
		Draw2D::Begin(Camera2D);
		UI::Begin();
	}

	void Renderer::End()
	{
		UI::End();
		Draw2D::End();
		RenderCommand::End();
	}

	void Renderer::Frame()
	{
		if (!Active)
			return;

		Begin();
		EventRendererRender event;
		Application::Dispatch(event);
		End();
	}
}
