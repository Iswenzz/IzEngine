target_precompile_headers(IW3SR.Engine PRIVATE
	Base.hpp)

target_sources(IW3SR.Engine PRIVATE
	Base.hpp
	Common.hpp
	Console.hpp
	Console.cpp
	Environment.hpp
	Environment.cpp
	KeyListener.hpp
	KeyListener.cpp
	Memory.hpp
	Memory.cpp
	Plugin.hpp
	Plugin.cpp
	Plugins.hpp
	Plugins.cpp
	Signature.hpp
	Signature.cpp
	Sys.hpp
	Sys.cpp)
