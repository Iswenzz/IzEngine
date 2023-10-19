cmake_minimum_required(VERSION 3.26)
project(IW3SR VERSION 1.0.0 LANGUAGES CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# NASM
if(WIN32)
	set(CMAKE_ASM_NASM_FLAGS "-dWin32 --prefix _")
	set(CMAKE_ASM_NASM_OBJECT_FORMAT "win32")
else()
	set(CMAKE_ASM_NASM_OBJECT_FORMAT "elf32")
endif()
enable_language(ASM_NASM)

# CPack
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR 7Z)
include(CPack)

# Dependencies
find_package(imgui CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(PolyHook_2 CONFIG REQUIRED)

# Tests
if(BUILD_TESTING)
	include(CTest)
	find_package(GTest CONFIG REQUIRED)
	add_library(IW3SR.Lib STATIC)
	add_executable(IW3SR.Tests)

	target_link_libraries(IW3SR.Lib PUBLIC IW3SR)
	target_include_directories(IW3SR.Lib PUBLIC IW3SR)

	target_link_libraries(IW3SR.Tests PRIVATE
		IW3SR.Lib
		GTest::gtest
		GTest::gtest_main
		GTest::gmock
		GTest::gmock_main)

	add_test(NAME Tests COMMAND IW3SR.Tests)
endif()

# Build
add_subdirectory(src)

set_target_properties(IW3SR PROPERTIES
	OUTPUT_NAME IW3SR
	SUFFIX ".asi")

target_link_libraries(IW3SR PUBLIC
	IW3SR.ASM
	imgui::imgui
	glm::glm
	PolyHook_2::PolyHook_2
	d3d9)

# Install
install(TARGETS IW3SR RUNTIME DESTINATION ${COD4}/miles)
install(DIRECTORY resources DESTINATION ${COD4}/iw3sr)
