add_subdirectory(Drawing)
add_subdirectory(Fonts)
add_subdirectory(UI)

target_precompile_headers(IW3SR.Engine PRIVATE
	Base.hpp)

target_sources(IW3SR.Engine PRIVATE
	Base.hpp
	Common.hpp
	Components.hpp
	Components.cpp
	UI.hpp
	UI.cpp)
