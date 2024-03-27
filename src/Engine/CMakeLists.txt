add_library(IW3SR.Engine OBJECT)

add_subdirectory(Core)
add_subdirectory(Platforms)
add_subdirectory(Renderer)

target_precompile_headers(IW3SR.Engine PRIVATE Base.hpp)
target_sources(IW3SR.Engine PRIVATE Base.hpp)

# Tests
if(BUILD_TESTING)
	add_executable(IW3SR.Engine.Tests)
	target_link_libraries(IW3SR.Engine.Tests PRIVATE
		IW3SR.Engine
		GTest::gtest
		GTest::gtest_main
		GTest::gmock
		GTest::gmock_main)
	add_test(NAME Engine COMMAND IW3SR.Engine.Tests)
endif()

# Build
target_include_directories(IW3SR.Engine PUBLIC . ..)
target_compile_definitions(IW3SR.Engine PUBLIC CMAKE_BINARY_DIR="${CMAKE_BINARY_DIR}")

target_link_libraries(IW3SR.Engine PUBLIC
	$<$<BOOL:${GRAPHICS_API_DXSDK}>:DirectX::SDK>
	imgui::imgui
	implot::implot
	nlohmann_json::nlohmann_json
	glm::glm
	asmjit::asmjit
	PolyHook_2::PolyHook_2)

# Tree
get_target_property(SOURCES IW3SR.Engine SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
