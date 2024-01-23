add_subdirectory(Drawing)
add_subdirectory(Fonts)
add_subdirectory(UI)

target_sources(IW3SR PRIVATE
	Components.hpp
	Components.cpp
	UI.hpp
	UI.cpp)
