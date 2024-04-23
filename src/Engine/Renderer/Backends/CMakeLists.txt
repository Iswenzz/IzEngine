add_subdirectory(ImGUI)

if(GRAPHICS_API_DX9)
	add_subdirectory(DX9)
endif()

target_include_directories(IzEngine PUBLIC .)
