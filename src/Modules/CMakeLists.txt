add_subdirectory(Player)
add_subdirectory(Sys)

target_sources(IW3SR PRIVATE
	Module.hpp
	Module.cpp
	Modules.hpp
	Modules.cpp)
