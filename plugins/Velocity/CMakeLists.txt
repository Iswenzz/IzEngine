# Build
add_library(Velocity SHARED main.cpp)
target_link_libraries(Velocity PRIVATE IW3SR.API)
set_target_properties(Velocity PROPERTIES FOLDER Plugins)

add_subdirectory(Modules)

get_target_property(SOURCES Velocity SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

# Install
install(TARGETS Velocity RUNTIME DESTINATION ${COD4}/iw3sr/plugins)
