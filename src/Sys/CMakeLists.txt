add_subdirectory(Fonts)
add_subdirectory(Render)

target_sources(IW3SR PRIVATE
	DLL.hpp
	DLL.cpp
	Environment.hpp
	Environment.cpp
	Function.hpp
	Hook.hpp
	Log.hpp
	Log.cpp
	Memory.hpp
	Memory.cpp)
