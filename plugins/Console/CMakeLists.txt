# Build
add_library(Console SHARED main.cpp)
target_link_libraries(Console PRIVATE IW3SR.API)
set_target_properties(Console PROPERTIES FOLDER Plugins)

add_subdirectory(Modules)

get_target_property(SOURCES Console SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

# Install
install(TARGETS Console RUNTIME DESTINATION ${COD4}/iw3sr/plugins)
