#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS

#include <d3dx9.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include <nlohmann/json.hpp>

#include "Game/Definitions.hpp"
#include "Game/Render/Fonts/IconsFontAwesome6.hpp"
#include "Game/Render/Fonts/IconsFontAwesome6Brands.hpp"

#include "Math/Math.hpp"

namespace ImGui
{
	/// <summary>
	/// Create a button with a specific id.
	/// </summary>
	/// <param name="label">The button label.</param>
	/// <param name="id">The unique id.</param>
	/// <param name="v">Toggle state.</param>
	/// <param name="size">The button size.</param>
	/// <returns></returns>
	API bool ButtonId(const std::string& label, const std::string& id, bool* v,
		const ImVec2& size = ImVec2(0, 0));

	/// <summary>
	/// Toggle button.
	/// </summary>
	/// <param name="id">The ID.</param>
	/// <param name="v">Toggle state.</param>
	/// <return></returns>
	API bool ToggleButton(const std::string& id, float size, bool* v);

	/// <summary>
	/// Creates a box group.
	/// </summary>
	/// <param name="name">The name of the box.</param>
	/// <param name="size">The size of the box.</param>
	/// <returns></returns>
	API void BeginGroupBox(const std::string& name, const ImVec2& size, const ImColor& color);

	/// <summary>
	/// Ends the group box.
	/// </summary>
	/// <returns></returns>
	API void EndGroupBox();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
