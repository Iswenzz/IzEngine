if(BUILD_TESTING)
	add_subdirectory(__test__)
endif()

target_sources(IzEngine PRIVATE
	CircularBuffer.hpp)
