add_library(IW3SR.Windows SHARED)
target_sources(IW3SR.Windows PRIVATE main.cpp)

# Build
target_link_libraries(IW3SR.Windows PUBLIC IW3SR.Engine IW3SR.Game)
set_target_properties(IW3SR.Windows PROPERTIES OUTPUT_NAME IW3SR SUFFIX ".asi")

# API
target_link_libraries(IW3SR.API INTERFACE IW3SR.Windows)
target_compile_definitions(IW3SR.API INTERFACE API=IMPORT)

# Tree
get_target_property(SOURCES IW3SR.Windows SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

# Debugger
if(MSVC)
	set_target_properties(IW3SR.Windows PROPERTIES
		VS_DEBUGGER_COMMAND $ENV{COD4}\\iw3mp.exe
		VS_DEBUGGER_WORKING_DIRECTORY $ENV{COD4})
endif()

# Install
install(TARGETS IW3SR.Windows RUNTIME DESTINATION ${COD4}/miles)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${COD4}/iw3sr)

install(DIRECTORY
	${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/$<$<CONFIG:Debug>:debug/>bin/
	DESTINATION ${COD4} FILES_MATCHING PATTERN "*.dll")
