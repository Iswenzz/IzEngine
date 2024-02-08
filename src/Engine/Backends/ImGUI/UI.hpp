#pragma once
#include "Engine/Backends/ImGUI/Components.hpp"
#include "Engine/Core/Screen/VirtualScreen.hpp"

#include "UI/Memory.hpp"
#include "UI/Themes.hpp"

namespace IW3SR::Engine
{
	/// <summary>
	/// UI class.
	/// </summary>
	class API UI : public IInitializable
	{
		CLASS_SINGLETON(UI)
	public:
		bool Active = false;
		bool Open = false;
		bool DesignMode = false;
		VirtualScreen Screen;
		float Size = 1.0f;
		float Scale = 1.0f;

		UC::Memory Memory;
		UC::Themes Themes;

		/// <summary>
		/// Initialize UI.
		/// </summary>
		void Initialize();

		/// <summary>
		/// Create the screen.
		/// </summary>
		/// <param name="position">The position.</param>
		/// <param name="size">The size.</param>
		/// <param name="display">The display size.</param>
		void CreateScreen(const vec2& position, const vec2& size, const vec2& display);

		/// <summary>
		/// Release UI.
		/// </summary>
		void Release();

		/// <summary>
		/// Begin frame.
		/// </summary>
		void Begin();

		/// <summary>
		/// End frame.
		/// </summary>
		void End();

		/// <summary>
		/// Initialize context.
		/// </summary>
		/// <remarks>Share context with DLLs.</remarks>
		static void InitializeContext();

		/// <summary>
		/// ImGUI allocator.
		/// </summary>
		/// <param name="size">The size.</param>
		/// <param name="data">The data.</param>
		/// <returns></returns>
		static void* Allocator(size_t size, void* data);

		/// <summary>
		/// ImGUI free.
		/// </summary>
		/// <param name="ptr">The pointer.</param>
		/// <param name="data">The data.</param>
		static void Free(void* ptr, void* data);

	private:
		void* Data = nullptr;
		ImGuiContext* Context = nullptr;
		ImPlotContext* PlotContext = nullptr;
		bool HasBegin = false;

		/// <summary>
		/// Initialize the UI.
		/// </summary>
		UI();
		virtual ~UI() = default;

		NLOHMANN_SERIALIZE(UI, Scale, Memory, Themes)
	};
}
