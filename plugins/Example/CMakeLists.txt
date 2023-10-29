# Build
add_library(Example SHARED main.cpp)
target_link_libraries(Example PRIVATE IW3SR.API)
set_target_properties(Example PROPERTIES FOLDER Plugins)

get_target_property(SOURCES Example SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

# Install
install(TARGETS Example RUNTIME DESTINATION ${COD4}/iw3sr/plugins)
