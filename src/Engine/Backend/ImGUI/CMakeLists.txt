add_subdirectory(Components)
add_subdirectory(Fonts)
add_subdirectory(UI)

target_precompile_headers(IzEngine PRIVATE
	Base.hpp
	Common.hpp)

target_sources(IzEngine PRIVATE
	Base.hpp
	Common.hpp)
