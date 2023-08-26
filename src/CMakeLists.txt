target_sources(IW3SR PRIVATE
	main.cpp)

target_include_directories(IW3SR PUBLIC .)
get_target_property(SOURCES IW3SR SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
