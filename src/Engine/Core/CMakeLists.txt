add_subdirectory(Application)
add_subdirectory(Base)
add_subdirectory(Communication)
add_subdirectory(Compilers)
add_subdirectory(Console)
add_subdirectory(Containers)
add_subdirectory(Input)
add_subdirectory(Interfaces)
add_subdirectory(Macros)
add_subdirectory(Math)
add_subdirectory(Memory)
add_subdirectory(Screen)
add_subdirectory(System)

if(BUILD_TESTING)
	add_subdirectory(Tests)
endif()

target_precompile_headers(IzEngine PRIVATE
	Base.hpp
	Math.hpp)

target_sources(IzEngine PRIVATE
	Base.hpp
	Math.hpp)
