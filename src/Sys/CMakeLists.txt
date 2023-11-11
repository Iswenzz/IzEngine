add_subdirectory(Listeners)
add_subdirectory(Memory)
add_subdirectory(Render)

target_sources(IW3SR PRIVATE
	DLL.hpp
	DLL.cpp
	Environment.hpp
	Environment.cpp
	Log.hpp
	Log.cpp
	Memory.hpp)
