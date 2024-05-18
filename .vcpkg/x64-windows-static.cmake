set(VCPKG_TARGET_TRIPLET x64-windows-static)

option(PLATFORM_WINDOWS "Build Windows" OFF)
if(PLATFORM_WINDOWS)
	list(APPEND VCPKG_MANIFEST_FEATURES "windows")
endif()

option(GRAPHICS_API_DX9 "Build DX9" OFF)
if(GRAPHICS_API_DX9)
	list(APPEND VCPKG_MANIFEST_FEATURES "dx9")
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
