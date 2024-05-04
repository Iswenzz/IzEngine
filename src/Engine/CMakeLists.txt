add_library(IzEngine OBJECT)

add_subdirectory(Core)
add_subdirectory(Platforms)
add_subdirectory(Renderer)

target_precompile_headers(IzEngine PRIVATE Base.hpp)
target_sources(IzEngine PRIVATE Base.hpp)

# Tests
if(BUILD_TESTING)
	add_executable(IzEngine.Tests)
	target_link_libraries(IzEngine.Tests PRIVATE
		IzEngine
		GTest::gtest
		GTest::gtest_main
		GTest::gmock
		GTest::gmock_main)
	add_test(NAME Engine COMMAND IzEngine.Tests)
endif()

# Build
target_include_directories(IzEngine PUBLIC . ..)

target_compile_definitions(IzEngine PUBLIC
	APPLICATION_ID="${APPLICATION_ID}"
	$<$<BOOL:${PLATFORM_WINDOWS}>:PLATFORM_WINDOWS>
	$<$<BOOL:${GRAPHICS_API_DX9}>:GRAPHICS_API_DX9>
	CMAKE_BINARY_DIR="${CMAKE_BINARY_DIR}"
	CMAKE_PROJECT_VERSION="${CMAKE_PROJECT_VERSION}")

target_link_libraries(IzEngine PUBLIC
	$<$<BOOL:${PLATFORM_WINDOWS}>:DbgHelp>
	$<$<BOOL:${GRAPHICS_API_DX9}>:DirectX::SDK>
	imgui::imgui
	implot::implot
	nlohmann_json::nlohmann_json
	glm::glm
	asmjit::asmjit
	PolyHook_2::PolyHook_2)

# Tree
get_target_property(SOURCES IzEngine SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
