add_library(IzEngine OBJECT)

add_subdirectory(Backend)
add_subdirectory(Core)
add_subdirectory(Platform)
add_subdirectory(Renderer)

# Build
target_precompile_headers(IzEngine PRIVATE Common.hpp)
target_sources(IzEngine PRIVATE Common.hpp)
target_include_directories(IzEngine PUBLIC ..)

target_compile_definitions(IzEngine PUBLIC
	APPLICATION_ID="${APPLICATION_ID}"
	APPLICATION_VERSION="${APPLICATION_VERSION}"
	$<$<BOOL:${ENABLE_API}>:ENABLE_API>
	$<$<BOOL:${PLATFORM_WINDOWS}>:PLATFORM_WINDOWS>
	$<$<BOOL:${GRAPHICS_API_DX9}>:GRAPHICS_API_DX9>
	CMAKE_BINARY_DIR="${CMAKE_BINARY_DIR}")

target_link_libraries(IzEngine PUBLIC
    asmjit::asmjit
    CURL::libcurl
    glm::glm
    imgui::imgui
    implot::implot
    libzippp::libzippp
    nlohmann_json::nlohmann_json
    PolyHook_2::PolyHook_2
    $<$<BOOL:${BUILD_CEF}>:CEF::CEF>
    $<$<BOOL:${GRAPHICS_API_DX9}>:DirectX::SDK>
    $<$<BOOL:${PLATFORM_WINDOWS}>:DbgHelp>
    $<$<BOOL:${PLATFORM_WINDOWS}>:dwmapi>)

# Tree
get_target_property(SOURCES IzEngine SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
