set(VCPKG_TARGET_TRIPLET x86-windows-static)

option(PLATFORM_WINDOWS "Build Windows" OFF)
if(PLATFORM_WINDOWS)
	list(APPEND VCPKG_MANIFEST_FEATURES "windows")
endif()

option(GRAPHICS_API_DX9 "Build DX9" OFF)
if(GRAPHICS_API_DX9)
	list(APPEND VCPKG_MANIFEST_FEATURES "dx9")
endif()

option(BUILD_CEF "Build CEF" OFF)
if(BUILD_CEF)
	list(APPEND VCPKG_MANIFEST_FEATURES "cef")
endif()

option(BUILD_OPENXR "Build OpenXR" OFF)
if(BUILD_OPENXR)
	list(APPEND VCPKG_MANIFEST_FEATURES "openxr")
endif()

option(BUILD_VULKAN "Build Vulkan" OFF)
if(BUILD_VULKAN)
	list(APPEND VCPKG_MANIFEST_FEATURES "vulkan")
endif()

option(BUILD_TESTING "Build Tests" OFF)
if(BUILD_TESTING)
	list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif()

if(NOT DEFINED ENV{VCPKG_ROOT})
	message(FATAL_ERROR "ERROR: VCPKG_ROOT environment variable is not defined. Please set it to the root directory of your vcpkg installation.")
endif()
file(TO_CMAKE_PATH $ENV{VCPKG_ROOT} VCPKG_ROOT)

include(${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
