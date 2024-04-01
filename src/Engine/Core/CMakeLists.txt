add_subdirectory(Application)
add_subdirectory(Base)
add_subdirectory(Communication)
add_subdirectory(Console)
add_subdirectory(Containers)
add_subdirectory(Input)
add_subdirectory(Interfaces)
add_subdirectory(Math)
add_subdirectory(Memory)
add_subdirectory(Screen)
add_subdirectory(System)

target_sources(IW3SR.Engine PRIVATE
	Common.hpp
	Math.hpp)
