add_subdirectory(Memory)
add_subdirectory(Win32)

target_precompile_headers(IW3SR PRIVATE
	Win32.hpp)

target_sources(IW3SR PRIVATE
	Memory.hpp
	Log.hpp
	Win32.hpp)
