target_sources(IzEngine PRIVATE
	Base.hpp
	Graphics.hpp
	OpenXR.hpp
	OpenXR.cpp)

if(PLATFORM_WINDOWS)
	target_sources(IzEngine PRIVATE
		D3D11.hpp
		D3D11.cpp)
endif()
