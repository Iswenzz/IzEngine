add_subdirectory(Base)
add_subdirectory(Communication)
add_subdirectory(Containers)
add_subdirectory(Interfaces)
add_subdirectory(Math)
add_subdirectory(Screen)

target_sources(IW3SR.Engine PRIVATE
	Common.hpp
	Math.hpp)
