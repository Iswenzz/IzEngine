add_subdirectory(Implementation)

target_sources(IW3SR.Engine PRIVATE
	Environment.hpp
	Plugin.hpp
	Plugins.hpp
	Plugins.cpp
	System.hpp)
