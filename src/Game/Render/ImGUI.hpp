#pragma once
#include "Game/Definitions.hpp"
#include "Utils/Math.hpp"

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
	bool ButtonId(const std::string& label, const std::string& id, bool* v, const ImVec2& size = ImVec2(0, 0));

	/// <summary>
	/// Toggle button.
	/// </summary>
	/// <param name="id">The ID.</param>
	/// <param name="v">Toggle state.</param>
	/// <return></returns>
	bool ToggleButton(const std::string& id, float size, bool* v);

	/// <summary>
	/// Set the window virtual rect.
	/// </summary>
	/// <param name="position">The position.</param>
	/// <param name="size">The size.</param>
	/// <param name="horizontal">The horizontal aligment.</param>
	/// <param name="vertical">The vertical alignment.</param>
	/// <return></returns>
	void SetWindowVirtual(vec2& position, vec2& size, RectAlignHorizontal_t horizontal, RectAlignVertical_t vertical);

	/// <summary>
	/// Set the window alignment.
	/// </summary>
	/// <param name="position">The position.</param>
	/// <param name="size">The size.</param>
	/// <param name="horizontal">The horizontal aligment.</param>
	/// <param name="vertical">The vertical alignment.</param>
	/// <return></returns>
	void SetWindowAlignment(vec2& position, const vec2& size, hudalign_t horizontal, hudalign_t vertical);
}
