add_subdirectory(Listeners)
add_subdirectory(Memory)
add_subdirectory(Modules)
add_subdirectory(Render)

target_sources(IW3SR PRIVATE
	Environment.hpp
	Environment.cpp
	Log.hpp
	Log.cpp
	Memory.hpp)
