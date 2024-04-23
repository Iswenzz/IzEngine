find_path(DXSDK_INCLUDE_DIRS "directxsdk/d3dx9.h")
find_library(D3DX9_LIB d3dx9)

set(DXSDK_LIBS
	$<$<BOOL:${GRAPHICS_API_DX9}>:${D3DX9_LIB}>
	$<$<BOOL:${GRAPHICS_API_DX9}>:d3d9>)

set(DXSDK_INCLUDE_DIRS
	${DXSDK_INCLUDE_DIRS}
	${DXSDK_INCLUDE_DIRS}/directxsdk)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DirectXSDK DEFAULT_MSG DXSDK_LIBS DXSDK_INCLUDE_DIRS)
mark_as_advanced(DXSDK_LIBS DXSDK_INCLUDE_DIRS)

if(NOT TARGET DirectX::SDK)
	add_library(DirectX::SDK INTERFACE IMPORTED)
    set_property(TARGET DirectX::SDK PROPERTY INTERFACE_LINK_LIBRARIES "${DXSDK_LIBS}")
	set_property(TARGET DirectX::SDK PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${DXSDK_INCLUDE_DIRS}")
endif()