add_subdirectory(Definitions)
add_subdirectory(Render)

target_sources(IW3SR PRIVATE
	Definitions.hpp
	Dvar.hpp
	Game.hpp
	Game.cpp
	Player.hpp
	Player.cpp)
