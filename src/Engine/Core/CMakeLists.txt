add_subdirectory(Containers)
add_subdirectory(Interfaces)
add_subdirectory(Math)
add_subdirectory(Screen)
add_subdirectory(Serializer)
add_subdirectory(Utils)

target_precompile_headers(IW3SR.Engine PRIVATE
	Interfaces.hpp
	Macros.hpp
	Math.hpp)

target_sources(IW3SR.Engine PRIVATE
	Interfaces.hpp
	Macros.hpp
	Math.hpp)
