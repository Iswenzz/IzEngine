add_subdirectory(Drawing)
add_subdirectory(Fonts)

target_sources(IW3SR PRIVATE
	Assets.hpp
	Assets.cpp
	GUI.hpp
	GUI.cpp
	ImGUI.hpp
	ImGUI.cpp
	Render.hpp
	Render.cpp)
