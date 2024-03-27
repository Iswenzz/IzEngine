cmake_minimum_required(VERSION 3.27)
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
find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(PolyHook_2 CONFIG REQUIRED)

# Platform
if(WIN32)
	set(PLATFORM_WINDOWS TRUE)
	set(GRAPHICS_API_DXSDK TRUE)
	set(GRAPHICS_API_DX9 TRUE)

	find_package(DirectXSDK REQUIRED)
	add_definitions(-DPLATFORM_WINDOWS -DGRAPHICS_API_DX9)
endif()

# Tests
if(BUILD_TESTING)
	include(CTest)
	find_package(GTest CONFIG REQUIRED)
endif()

# Build
add_subdirectory(src)
add_subdirectory(plugins)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/private")
    add_subdirectory(private)
endif()

# Clang
add_custom_target(clang-tidy COMMAND clang-tidy -p ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR})
add_custom_target(clang-format COMMAND clang-format -i ${CMAKE_SOURCE_DIR})
set_target_properties(clang-tidy PROPERTIES FOLDER Clang)
set_target_properties(clang-format PROPERTIES FOLDER Clang)
