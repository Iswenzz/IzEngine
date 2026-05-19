add_subdirectory(Base)
add_subdirectory(Camera)
add_subdirectory(Drawing)
add_subdirectory(UI)

target_precompile_headers(IzEngine PRIVATE Common.hpp)
target_sources(IzEngine PRIVATE Common.hpp)
