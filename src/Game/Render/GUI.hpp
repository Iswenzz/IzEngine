#pragma once
#include "Game/Definitions.hpp"
#include "Drawing/Window.hpp"
#include "Sys/Listeners/KeyListener.hpp"

#include "UI/About.hpp"
#include "UI/Binds.hpp"
#include "UI/Settings.hpp"
#include "UI/Themes.hpp"
#include "UI/Toolbar.hpp"

#include <tuple>

namespace IW3SR
{
	/// <summary>
	/// ImGUI class.
	/// </summary>
	class API GUI
	{
	public:
		static inline ImGuiContext* Context = nullptr;
		static inline void* Data = nullptr;

		static inline HWND MainWindow = nullptr;
		static inline Toolbar Toolbar;
		static inline KeyListener OpenKey;
		static inline bool Active = false;
		static inline bool Open = false;
		static inline bool DesignMode = false;

		static inline std::tuple<ImColor, ImColor> ColorRainbow;

		/// <summary>
		/// Initialize ImGUI.
		/// </summary>
		GUI();
		~GUI() = default;

		/// <summary>
		/// Patch the game.
		/// </summary>
		void Patch();

		/// <summary>
		/// Initialize ImGUI.
		/// </summary>
		static void Initialize();

		/// <summary>
		/// Shutdown ImGUI.
		/// </summary>
		static void Shutdown();

		/// <summary>
		/// Reset ImGUI.
		/// </summary>
		static void Reset();

		/// <summary>
		/// Reset the mouse.
		/// </summary>
		static void ResetMouse();

		/// <summary>
		/// Begin frame.
		/// </summary>
		static void Begin();

		/// <summary>
		/// End frame.
		/// </summary>
		static void End();

		/// <summary>
		/// Render frame.
		/// </summary>
		static void Frame();

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
		/// <summary>
		/// Compute the rainbow effect.
		/// </summary>
		static void Rainbow();

		/// <summary>
		/// GUI theme.
		/// </summary>
		static void Theme();
	};
}
