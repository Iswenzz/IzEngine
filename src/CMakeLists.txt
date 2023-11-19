add_library(IW3SR OBJECT)
add_library(IW3SR.API INTERFACE)
add_library(IW3SR.ASM OBJECT)
add_library(IW3SR.DLL SHARED)

add_subdirectory(Game)
add_subdirectory(Math)
add_subdirectory(Modules)
add_subdirectory(Sys)
add_subdirectory(Utils)

target_include_directories(IW3SR PUBLIC .)
target_sources(IW3SR.DLL PRIVATE main.cpp)
target_link_libraries(IW3SR.DLL PUBLIC IW3SR)
target_compile_definitions(IW3SR PUBLIC CMAKE_BINARY_DIR="${CMAKE_BINARY_DIR}")

# API
target_link_libraries(IW3SR.API INTERFACE IW3SR.DLL)
target_compile_definitions(IW3SR.API INTERFACE API=IMPORT)

# Tree
get_target_property(SOURCES_IW3SR IW3SR SOURCES)
get_target_property(SOURCES_IW3SR_DLL IW3SR.DLL SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES_IW3SR} ${SOURCES_IW3SR_DLL})

# Debugger
if(MSVC)
	set_target_properties(IW3SR PROPERTIES
		VS_DEBUGGER_COMMAND $ENV{COD4}\\iw3mp.exe
		VS_DEBUGGER_WORKING_DIRECTORY $ENV{COD4})
endif()
