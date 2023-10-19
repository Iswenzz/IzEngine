#pragma once
#include <imgui.h>
#include <string>

namespace ImGui
{
	/// <summary>
	/// Create a button with a specific id.
	/// </summary>
	/// <param name="label">The button label.</param>
	/// <param name="id">The unique id.</param>
	/// <param name="size">The button size.</param>
	/// <returns></returns>
	bool ButtonId(const std::string& label, const std::string& id, const ImVec2& size = ImVec2(0, 0));

	/// <summary>
	/// Toggle button.
	/// </summary>
	/// <param name="id">The ID.</param>
	/// <param name="v">Toggle state.</param>
	/// <return></returns>
	bool ToggleButton(const std::string& id, float size, bool* v);
}
