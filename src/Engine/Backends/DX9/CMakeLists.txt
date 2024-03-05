add_subdirectory(Drawing)
add_subdirectory(Resources)

target_sources(IW3SR.Engine PRIVATE
	Assets.hpp
	Assets.cpp
	Device.hpp
	Device.cpp)
