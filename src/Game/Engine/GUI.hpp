#pragma once
#include "Game/Definitions.hpp"

#include "Engine/Backends/ImGUI/Drawing/Window.hpp"
#include "Engine/Backends/ImGUI/Drawing/Plots.hpp"

#include "UI/About.hpp"
#include "UI/Binds.hpp"
#include "UI/Memory.hpp"
#include "UI/Modules.hpp"
#include "UI/Settings.hpp"
#include "UI/Themes.hpp"
#include "UI/Toolbar.hpp"

namespace IW3SR
{
	/// <summary>
	/// GUI class.
	/// </summary>
	class API GUI
	{
	public:
		static inline ImGuiContext* Context = nullptr;
		static inline ImPlotContext* PlotContext = nullptr;
		static inline void* Data = nullptr;

		HWND MainWindow = nullptr;
		bool Active = false;
		bool Open = false;
		bool DesignMode = false;

		KeyListener OpenKey;
		UI::About About;
		UI::Binds Binds;
		UI::Memory Memory;
		UI::Modules Modules;
		UI::Settings Settings;
		UI::Themes Themes;
		UI::Toolbar Toolbar;

		/// <summary>
		/// Initialize
		/// </summary>
		GUI();
		~GUI();

		/// <summary>
		/// Patch the game.
		/// </summary>
		void Patch();

		/// <summary>
		/// Initialize GUI.
		/// </summary>
		void Initialize();

		/// <summary>
		/// Shutdown GUI.
		/// </summary>
		void Shutdown();

		/// <summary>
		/// Reset GUI.
		/// </summary>
		void Reset();

		/// <summary>
		/// Reset the mouse.
		/// </summary>
		void ResetMouse();

		/// <summary>
		/// Begin frame.
		/// </summary>
		void Begin();

		/// <summary>
		/// End frame.
		/// </summary>
		void End();

		/// <summary>
		/// Render frame.
		/// </summary>
		void Render();

		/// <summary>
		/// Creates the main window.
		/// </summary>
		/// <param name="dwExStyle">The extended window style.</param>
		/// <param name="lpClassName">The window class name.</param>
		/// <param name="lpWindowName">The window title.</param>
		/// <param name="dwStyle">The window style.</param>
		/// <param name="X">The X-coordinate of the window.</param>
		/// <param name="Y">The Y-coordinate of the window.</param>
		/// <param name="nWidth">The width of the window.</param>
		/// <param name="nHeight">The height of the window.</param>
		/// <param name="hWndParent">The parent window handle.</param>
		/// <param name="hMenu">The menu handle.</param>
		/// <param name="hInstance">The instance handle.</param>
		/// <param name="lpParam">A pointer to user-defined data.</param>
		/// <returns>The handle to the created main window.</returns>
		static HWND STDCALL CreateMainWindow(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName,
			DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu,
			HINSTANCE hInstance, LPVOID lpParam);

		/// <summary>
		/// Window procedure for the main window.
		/// </summary>
		/// <param name="hWnd">The handle to the window.</param>
		/// <param name="Msg">The message code.</param>
		/// <param name="wParam">Additional message-specific information.</param>
		/// <param name="lParam">Additional message-specific information.</param>
		/// <returns>The result of processing the message.</returns>
		static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

		/// <summary>
		/// Set the module context.
		/// </summary>
		/// <remarks>This can be called on dynamic libraries to share context.</remarks>
		static void ModuleContext();

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

		NLOHMANN_SERIALIZE(GUI, About, Binds, Memory, Modules, Settings, Themes, OpenKey)
	};
}
