add_library(IW3SR.Game OBJECT)
add_library(IW3SR.API INTERFACE)

add_subdirectory(Definitions)
add_subdirectory(Platforms)
add_subdirectory(Player)
add_subdirectory(Renderer)
add_subdirectory(Sys)
add_subdirectory(World)

target_precompile_headers(IW3SR.Game PRIVATE
	Definitions.hpp
	Game.hpp
	Plugin.hpp)

target_sources(IW3SR.Game PRIVATE
	Definitions.hpp
	Game.hpp
	Game.cpp
	Plugin.hpp)

# Tests
if(BUILD_TESTING)
	add_executable(IW3SR.Game.Tests)
	add_test(NAME Game COMMAND IW3SR.Game.Tests)
endif()

# Build
target_include_directories(IW3SR.Game PUBLIC ..)
target_link_libraries(IW3SR.Game PUBLIC IW3SR.Engine)

# Tree
get_target_property(SOURCES IW3SR.Game SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
