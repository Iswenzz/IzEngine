add_subdirectory(Drawing)
add_subdirectory(Resources)

target_precompile_headers(IW3SR.Engine PRIVATE
	Base.hpp)

target_sources(IW3SR.Engine PRIVATE
	Assets.hpp
	Assets.cpp
	Common.hpp
	Base.hpp
	Device.hpp
	Device.cpp)
