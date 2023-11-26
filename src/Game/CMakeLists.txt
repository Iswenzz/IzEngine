add_subdirectory(Definitions)
add_subdirectory(Engine)
add_subdirectory(Player)
add_subdirectory(Render)
add_subdirectory(Sys)

target_precompile_headers(IW3SR PRIVATE
	Game.hpp)

target_sources(IW3SR PRIVATE
	Definitions.hpp
	Game.hpp
	Game.cpp)
