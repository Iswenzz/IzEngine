cmake_minimum_required(VERSION 3.27)
project(IzEngine VERSION 1.0.0 LANGUAGES CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# CMake
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(ClangTools)

# Compilers
if(MSVC)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
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
if(PLATFORM_WINDOWS)
	add_definitions(-DPLATFORM_WINDOWS)
endif()

# Backends
if(GRAPHICS_API_DX9)
	find_package(DirectXSDK REQUIRED)
	add_definitions(-DGRAPHICS_API_DX9)
endif()

# Tests
if(BUILD_TESTING)
	include(CTest)
	find_package(GTest CONFIG REQUIRED)
endif()

# Build
add_subdirectory(src)
