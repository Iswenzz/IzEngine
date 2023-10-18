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
	/// Create a button to toggle on/off.
	/// </summary>
	/// <param name="id">the ID/name.</param>
	/// <param name="v">Enabled or not (false by default).</param>
	void ToggleButton(const std::string& id, float size, bool* v = false);
}
