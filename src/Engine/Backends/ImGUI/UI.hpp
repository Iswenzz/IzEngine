#pragma once
#include "Engine/Sys/Win32.hpp"

#include "UI/Memory.hpp"
#include "UI/Modules.hpp"
#include "UI/Settings.hpp"
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
		bool HasBegin = false;
		KeyListener OpenKey;

		UC::Memory Memory;
		UC::Modules Modules;
		UC::Settings Settings;
		UC::Themes Themes;

		/// <summary>
		/// Initialize UI.
		/// </summary>
		void Initialize();

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
		ImGuiContext* Context = nullptr;
		ImPlotContext* PlotContext = nullptr;
		void* Data = nullptr;

		/// <summary>
		/// Initialize the UI.
		/// </summary>
		UI();
		virtual ~UI() = default;

		NLOHMANN_SERIALIZE(UI, OpenKey, Memory, Modules, Settings, Themes)
	};
}
