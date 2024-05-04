add_subdirectory(Implementation)

target_sources(IzEngine PRIVATE
	Crash.hpp
	Environment.hpp
	Plugin.hpp
	Plugins.hpp
	Plugins.cpp
	System.hpp)
