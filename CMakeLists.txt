cmake_minimum_required(VERSION 3.26)
project(IW3SR VERSION 1.0.0 LANGUAGES CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# CMake
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Compilers
if(MSVC)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
	set(CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD true)
endif()

# CPack
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR 7Z)
include(CPack)

# Dependencies
find_package(DirectXSDK REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(PolyHook_2 CONFIG REQUIRED)

# Tests
if(BUILD_TESTING)
	include(CTest)
	find_package(GTest CONFIG REQUIRED)

	target_link_libraries(IW3SR.Tests PUBLIC
		GTest::gtest
		GTest::gtest_main
		GTest::gmock
		GTest::gmock_main)
endif()

# Build
add_subdirectory(src)
add_subdirectory(plugins)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/private")
    add_subdirectory(private)
endif()
