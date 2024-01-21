add_subdirectory(Containers)
add_subdirectory(Features)
add_subdirectory(Interfaces)
add_subdirectory(Math)
add_subdirectory(Modules)
add_subdirectory(Serializer)
add_subdirectory(Utils)

target_precompile_headers(IW3SR PRIVATE
	Interfaces.hpp
	Macros.hpp
	Math.hpp
	Modules.hpp)

target_sources(IW3SR PRIVATE
	Interfaces.hpp
	Macros.hpp
	Math.hpp
	Modules.hpp)
