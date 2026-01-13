cmake_minimum_required(VERSION 4.2)
project(IzEngine VERSION 1.0.0 LANGUAGES CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# CMake
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(ClangTools)

# Dependencies
find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(PolyHook_2 CONFIG REQUIRED)

if(GRAPHICS_API_DX9)
	find_package(DirectXSDK REQUIRED)
endif()

if(BUILD_CEF)
	find_package(CEF REQUIRED)
endif()

# Tests
if(BUILD_TESTING)
	include(CTest)
	find_package(GTest CONFIG REQUIRED)
	add_executable(IzEngine.Tests)
	add_test(NAME IzEngine COMMAND IzEngine.Tests)

	target_link_libraries(IzEngine.Tests PRIVATE
		IzEngine
		GTest::gtest
		GTest::gtest_main
		GTest::gmock
		GTest::gmock_main)
endif()

# Build
add_subdirectory(src)
