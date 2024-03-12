add_subdirectory(Memory)

if(PLATFORM_WINDOWS)
	add_subdirectory(Windows)
endif()

target_sources(IW3SR.Engine PRIVATE
	Log.hpp)

target_include_directories(IW3SR.Engine PUBLIC .)
