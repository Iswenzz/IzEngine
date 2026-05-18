target_precompile_headers(IzEngine PRIVATE
	Base.hpp)

target_sources(IzEngine PRIVATE
	AssetManager.cpp
	Base.hpp
	Console.cpp
	Crash.cpp
	Environment.cpp
	Input.cpp
	Keyboard.cpp
	Memory.cpp
	Mouse.cpp
	Plugin.cpp
	Signature.cpp
	System.cpp
	Window.cpp)
