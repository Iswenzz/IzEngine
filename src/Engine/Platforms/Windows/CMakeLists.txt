target_precompile_headers(IW3SR.Engine PRIVATE
	Base.hpp)

target_sources(IW3SR.Engine PRIVATE
	Base.hpp
	Console.cpp
	Environment.cpp
	KeyListener.cpp
	Memory.cpp
	Plugin.cpp
	Plugins.cpp
	Signature.cpp
	System.cpp)
