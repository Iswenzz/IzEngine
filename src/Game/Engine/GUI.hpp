#pragma once
#include "Engine/Backends/ImGUI/UI.hpp"

#include "UI/About.hpp"
#include "UI/Binds.hpp"
#include "UI/Toolbar.hpp"

namespace IW3SR::Game
{
	/// <summary>
	/// GUI class.
	/// </summary>
	class API GUI : public IInitializable
	{
		CLASS_SINGLETON(GUI)
	public:
		Engine::UI& UI;

		UC::About About;
		UC::Binds Binds;
		UC::Toolbar Toolbar;

		/// <summary>
		/// Initialize GUI.
		/// </summary>
		void Initialize();

		/// <summary>
		/// Release GUI.
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
		/// Render frame.
		/// </summary>
		void Render();

	private:
		/// <summary>
		/// Initialize the GUI.
		/// </summary>
		GUI();
		virtual ~GUI();

		NLOHMANN_SERIALIZE(GUI, About, Binds, Toolbar)
	};
}
