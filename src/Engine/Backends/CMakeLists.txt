add_subdirectory(DX9)
add_subdirectory(ImGUI)

target_precompile_headers(IW3SR PRIVATE
	DX9.hpp
	ImGUI.hpp)

target_sources(IW3SR PRIVATE
	DX9.hpp
	ImGUI.hpp)
