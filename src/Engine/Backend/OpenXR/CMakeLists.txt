target_sources(IzEngine PRIVATE
	Base.hpp
	Graphics.hpp
	Graphics.cpp
	OpenXR.hpp
	OpenXR.cpp
	Platform.hpp)

if(PLATFORM_WINDOWS)
	target_sources(IzEngine PRIVATE
		Direct3D.hpp
		Direct3D.cpp
		D3D11.hpp
		D3D11.cpp
		D3D12.hpp
		D3D12.cpp)
endif()

if(BUILD_VULKAN)
	target_sources(IzEngine PRIVATE
		Vulkan.hpp
		Vulkan.cpp)
endif()
