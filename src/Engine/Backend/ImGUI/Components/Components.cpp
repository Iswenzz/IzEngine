#include "Components.hpp"

#include "Engine/Backend/ImGUI/Common.hpp"

namespace
{
	// Drawn over whatever the window is, so they read on every theme.
	constexpr auto SectionHeaderColor = ImVec4(0.00f, 0.00f, 0.00f, 0.45f);
	constexpr auto SectionHeaderHoveredColor = ImVec4(0.00f, 0.00f, 0.00f, 0.35f);
	constexpr auto SectionBodyColor = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);

	constexpr float PropertyLabelRatio = 0.42f;

	float SectionRounding()
	{
		return ImGui::GetFontSize() * 0.3f;
	}

	vec2 SectionPadding()
	{
		const ImGuiStyle& style = ImGui::GetStyle();
		return { style.ItemSpacing.x * 1.25f, style.ItemSpacing.y };
	}
}

namespace ImGui
{
	MarkdownConfig MarkConfig;
	std::tuple<ImColor, ImColor> RainbowTuple;

	ImFont* H1 = nullptr;
	ImFont* H2 = nullptr;
	ImFont* H3 = nullptr;

	void PushID(const UUID& uuid)
	{
		PushID(uuid.String.c_str());
	}

	bool Button(const std::string& label, bool* v, const vec2& size)
	{
		Button(label.c_str(), size);

		bool clicked = IsItemClicked();
		if (v && clicked)
			*v = !*v;
		return clicked;
	}

	bool Button(const std::string& label, const std::string& id, bool* v, const vec2& size)
	{
		PushID(id.c_str());
		Button(label.c_str(), size);
		PopID();

		bool clicked = IsItemClicked();
		if (v && clicked)
			*v = !*v;
		return clicked;
	}

	bool ButtonToggle(const std::string& label, const std::string& id, bool* v, const vec2& size)
	{
		if (*v)
		{
			PushStyleColor(ImGuiCol_Button, GetStyleColorVec4(ImGuiCol_ButtonActive));
			PushStyleColor(ImGuiCol_ButtonHovered, GetStyleColorVec4(ImGuiCol_ButtonActive));
		}
		PushID(id.c_str());
		Button(label.c_str(), size);
		PopID();

		if (*v)
			PopStyleColor(2);

		bool clicked = IsItemClicked();
		if (clicked)
			*v = !*v;
		return clicked;
	}

	bool Toggle(const std::string& id, bool* v, const vec2& defaultSize)
	{
		ImGuiContext& g = *GImGui;
		ImGuiStyle& style = g.Style;
		vec2 p = GetCursorScreenPos();
		ImDrawList* draw = GetWindowDrawList();

		float fontSize = g.FontSize;
		float height = fontSize + style.FramePadding.y / 2.0f;
		float width = height + style.FramePadding.x * 2.5f;
		float radius = height * 0.5f;
		vec2 size = CalcItemSize(defaultSize, width, height);

		InvisibleButton(id.c_str(), size);
		bool clicked = IsItemClicked();
		if (clicked)
			*v = !*v;

		const float ANIM_SPEED = 0.08f;
		float t = *v ? 1.0f : 0.0f;

		if (g.LastActiveId == g.CurrentWindow->GetID(id.c_str()))
		{
			float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
			t = *v ? (t_anim) : (1.0f - t_anim);
		}
		const ImU32 color = IsItemHovered()
			? GetColorU32(ImLerp({ 0.78f, 0.78f, 0.78f, 1.0f }, { 0.64f, 0.83f, 0.34f, 1.0f }, t))
			: GetColorU32(ImLerp({ 0.85f, 0.85f, 0.85f, 1.0f }, { 0.56f, 0.83f, 0.26f, 1.0f }, t));
		const ImU32 white = GetColorU32({ 1, 1, 1, 1 });

		draw->AddRectFilled(p, { p.x + size.x, p.y + size.y }, color, size.y * 0.5f);
		draw->AddCircleFilled({ p.x + radius + t * (size.x - radius * 2.0f), p.y + radius }, radius - 1.5f, white);

		return clicked;
	}

	bool Combo(const std::string& label, int* item, const std::vector<std::string>& items, int maxHeight)
	{
		std::vector<const char*> list;
		for (const auto& item : items)
			list.push_back(item.c_str());

		return Combo(label.c_str(), item, list.data(), list.size(), maxHeight);
	}

	bool ComboAlign(Alignment* x, Alignment* y)
	{
		bool state = false;
		int index = static_cast<int>(*x) / 4;
		Property("Align X");
		if (Combo("##alignx", &index, Horizontals.data(), Horizontals.size()))
		{
			*x = static_cast<Alignment>(index * 4);
			state = true;
		}
		index = static_cast<int>(*y);
		Property("Align Y");
		if (Combo("##aligny", &index, Verticals.data(), Verticals.size()))
		{
			*y = static_cast<Alignment>(index);
			state = true;
		}
		return state;
	}

	bool ComboAlignRect(Horizontal* horizontal, Vertical* vertical)
	{
		bool state = false;
		int index = static_cast<int>(*horizontal);
		Property("Horizontal");
		if (Combo("##horizontal", &index, HorizontalAnchors.data(), HorizontalAnchors.size()))
		{
			*horizontal = static_cast<Horizontal>(index);
			state = true;
		}
		index = static_cast<int>(*vertical);
		Property("Vertical");
		if (Combo("##vertical", &index, VerticalAnchors.data(), VerticalAnchors.size()))
		{
			*vertical = static_cast<Vertical>(index);
			state = true;
		}
		return state;
	}

	bool CollapsingHeader(const std::string& label, bool open, ImGuiTreeNodeFlags flags)
	{
		flags |= open ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None;
		return CollapsingHeader(label.c_str(), flags);
	}

	// A Blender-style panel: a header bar that folds it, over a padded body. Returns whether it is
	// open; only then is EndSection called. The open state is kept per title in the window's storage.
	bool BeginSection(const std::string& title, bool open)
	{
		const ImGuiStyle& style = GetStyle();
		ImDrawList* draw = GetWindowDrawList();
		ImGuiStorage* storage = GetStateStorage();

		PushID(title.c_str());
		const ImGuiID id = GetID("##open");
		bool expanded = storage->GetBool(id, open);

		const vec2 min = GetCursorScreenPos();
		const float width = GetContentRegionAvail().x;
		const float height = GetFrameHeight() + style.FramePadding.y;

		if (InvisibleButton("##header", { width, height }))
		{
			expanded = !expanded;
			storage->SetBool(id, expanded);
		}
		const ImVec4 color = IsItemHovered() ? SectionHeaderHoveredColor : SectionHeaderColor;
		draw->AddRectFilled(min, { min.x + width, min.y + height }, GetColorU32(color), SectionRounding(),
			expanded ? ImDrawFlags_RoundCornersTop : ImDrawFlags_RoundCornersAll);

		const float iconSize = GetFontSize() * 0.7f;
		const float iconX = min.x + SectionPadding().x;
		draw->AddText(GetFont(), iconSize, { iconX, min.y + (height - iconSize) * 0.5f },
			GetColorU32(ImGuiCol_TextDisabled), expanded ? ICON_FA_CHEVRON_DOWN : ICON_FA_CHEVRON_RIGHT);
		draw->AddText({ iconX + iconSize + style.ItemInnerSpacing.x * 2.0f, min.y + (height - GetFontSize()) * 0.5f },
			GetColorU32(ImGuiCol_Text), title.c_str());

		if (!expanded)
		{
			PopID();
			Dummy({ 0, style.ItemSpacing.y * 0.25f });
			return false;
		}

		// The body sits flush under the header rather than an item spacing below it.
		SetCursorScreenPos({ min.x, min.y + height });
		PushStyleVar(ImGuiStyleVar_WindowPadding, SectionPadding());
		BeginChild("##body", { width, 0 }, ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysUseWindowPadding,
			ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		PopStyleVar();
		return true;
	}

	// The body's background goes into the parent's draw list after the child has closed: a child
	// window renders after its parent, so it still lands underneath the rows.
	void EndSection()
	{
		EndChild();

		const vec2 min = GetItemRectMin();
		const vec2 max = GetItemRectMax();
		GetWindowDrawList()->AddRectFilled(min, max, GetColorU32(SectionBodyColor), SectionRounding(),
			ImDrawFlags_RoundCornersBottom);

		PopID();
		Dummy({ 0, GetStyle().ItemSpacing.y * 0.25f });
	}

	// Starts a row: the label right-aligned in the left column, and the next item sized to the rest
	// of the row, less the width asked for something drawn after it on the same line.
	void Property(const std::string& label, float trailing)
	{
		const ImGuiStyle& style = GetStyle();
		const float start = GetCursorPosX();
		const float column = std::floor(GetContentRegionAvail().x * PropertyLabelRatio);
		const float text = CalcTextSize(label.c_str(), nullptr, true).x;

		AlignTextToFramePadding();
		SetCursorPosX(start + std::max(0.0f, column - style.ItemInnerSpacing.x * 2.0f - text));
		TextUnformatted(label.c_str());

		SameLine(start + column);
		SetNextItemWidth(trailing > 0 ? -(trailing + style.ItemSpacing.x) : -FLT_MIN);
	}

	// A toggle switch centred on the row's frame height, so it lines up with its label.
	bool Switch(const std::string& id, bool* v)
	{
		const float height = GetFontSize() + GetStyle().FramePadding.y * 0.5f;
		SetCursorPosY(GetCursorPosY() + (GetFrameHeight() - height) * 0.5f);

		const bool before = *v;
		Toggle(id, v, { height * 1.9f, height });
		return *v != before;
	}

	void Tooltip(const std::string& text)
	{
		if (!IsItemHovered())
			return;

		vec2 size = GetItemRectSize();
		vec2 position = GetItemRectMin();
		position.y += size.y + 2;

		SetNextWindowPos(position);
		PushStyleVar(ImGuiStyleVar_WindowPadding, { 6, 6 });

		BeginTooltip();
		TextUnformatted(text.c_str());
		EndTooltip();

		PopStyleVar();
	}

	void Keybind(const std::string& label, InputEnum* key, bool unbind, const vec2& defaultSize)
	{
		const auto keyName = Input::GetName(*key);
		const auto id = GetID(label.c_str());

		float width = 50 * UI::Size;
		vec2 size = CalcItemSize(defaultSize, width, 0);

		KeepAliveID(id);
		PushID(id);

		if (GetActiveID() == id)
		{
			PushStyleColor(ImGuiCol_Button, GetColorU32(ImGuiCol_ButtonActive));
			Button("...", size);
			PopStyleColor();

			// Cancelling comes first: the click that cancels is itself a press of Left Click, and would
			// otherwise be bound. A mouse button only binds while over the button it is binding.
			const bool hovered = IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
			if (Input::IsPressed(Key_Escape) || (!hovered && IsMouseClicked(ImGuiMouseButton_Left)))
				ClearActiveID();
			else
			{
				for (int k = Input_None + 1; k < Input_Count; k++)
				{
					if (!hovered && k >= Button_Left)
						continue;
					if (Input::IsPressed(static_cast<InputEnum>(k)))
					{
						*key = static_cast<InputEnum>(k);
						ClearActiveID();
						break;
					}
				}
			}
		}
		else
		{
			Button(keyName, size);
			if (IsItemClicked(ImGuiMouseButton_Left))
				SetActiveID(id, GetCurrentWindow());
			else if (IsItemClicked(ImGuiMouseButton_Right) && unbind)
				*key = Input_None;
		}
		if (!label.starts_with("##"))
		{
			SameLine();
			TextUnformatted(label.c_str());
		}
		PopID();
	}

	void Movable(const UUID& id, vec2& position, vec2& size, vec2& renderPosition, vec2& renderSize)
	{
		if (!UI::Open || !UI::DesignMode)
			return;

		PushStyleColor(ImGuiCol_Border, { 0.2, 0.2, 0.2, 0.2 });
		SetNextWindowBgAlpha(0.2f);
		SetNextWindowPos(renderPosition, ImGuiCond_FirstUseEver);
		SetNextWindowSize(renderSize, ImGuiCond_FirstUseEver);
		Begin(id.String.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);

		const vec2 framePosition = GetWindowPos();
		const vec2 frameSize = GetWindowSize();

		if (IsMoving())
		{
			position += UI::Screen.RealToVirtual * (framePosition - renderPosition);
			size += UI::Screen.RealToVirtual * (frameSize - renderSize);

			renderPosition = framePosition;
			renderSize = frameSize;
		}
		else
		{
			SetWindowPos(renderPosition);
			SetWindowSize(renderSize);
		}
		End();
		PopStyleColor();
	}

	void ComputeRainbow()
	{
		const float speed = 0.15f;
		static float offset = 0;

		RainbowTuple = std::make_tuple(ImColor::HSV(fmod(offset, 1.0f), 1.0f, 1.0f),
			ImColor::HSV(fmod(offset + 0.33f, 1.0f), 1.0f, 1.0f));
		offset += speed * UI::DeltaTime();
	}

	void Rainbow(const vec2& position, const vec2& size)
	{
		ImDrawList* draw = GetForegroundDrawList();

		const auto& [rainbow1, rainbow2] = RainbowTuple;
		draw->AddRectFilledMultiColor(position, position + size, rainbow1, rainbow2, rainbow2, rainbow1);
	}

	void Markdown(const std::string& markdown)
	{
		Markdown(markdown.c_str(), markdown.size(), MarkConfig);
	}

	void LoadingIndicator(const std::string& label, const vec2& pos, const ImU32& color, bool state)
	{
		if (!state)
			return;

		ImDrawList* draw = GetBackgroundDrawList();
		const ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;

		float fontSize = g.FontSize;
		float height = fontSize + style.FramePadding.y;
		float width = height + style.FramePadding.x * 3.f;
		float radius = height / 2.f;
		float thickness = 5;

		const auto size = CalcItemSize({ 0, 0 }, width, height);
		const auto position = UI::Screen.VirtualToFull * pos;

		const float segments = 15;
		const float start = abs(sinf(g.Time * 1.8f) * (segments - 5));
		const float aMin = IM_PI * 2.0f * start / segments;
		const float aMax = IM_PI * 2.0f * (segments - 3) / segments;

		for (int i = 0; i < segments; i++)
		{
			const float a = aMin + (i / segments) * (aMax - aMin);
			draw->PathLineTo(
				{ position.x + cosf(a + g.Time * 8) * radius, position.y + sinf(a + g.Time * 8) * radius });
		}
		draw->PathStroke(color, 0, thickness);
		draw->AddText(position + vec2{ size.x, -radius }, color, label.c_str());
	}

	bool IsResizing(ImGuiWindow* window)
	{
		ImGuiID active = GetActiveID();

		if (!window || !active)
			return false;

		for (int i = 0; i < 4; i++)
		{
			if (GetWindowResizeCornerID(window, static_cast<ImGuiDir>(i)) == active)
				return true;
			if (GetWindowResizeBorderID(window, static_cast<ImGuiDir>(i)) == active)
				return true;
		}
		return false;
	}

	bool IsResizing()
	{
		return IsResizing(GetCurrentWindow());
	}

	bool IsMoving()
	{
		if (glm::length2(vec2(GetMouseDragDelta())) && (IsWindowHovered() || IsResizing()))
		{
			ResetMouseDragDelta();
			return true;
		}
		return false;
	}

	// True while ImGui itself owns the window rect: the user is dragging the window or a resize grip.
	// Unlike IsMoving this holds for the whole interaction, including frames where the mouse is still.
	bool IsInteracting(ImGuiWindow* window)
	{
		if (!window)
			return false;

		const ImGuiContext& g = *GImGui;
		if (g.MovingWindow && g.MovingWindow->RootWindow == window->RootWindow)
			return true;

		return IsResizing(window);
	}
}
