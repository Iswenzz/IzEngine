add_subdirectory(Draw)

target_sources(IW3SR PRIVATE
	Draw.hpp
	Draw.cpp
	Draw2D.hpp
	Draw2D.cpp
	Draw3D.hpp
	Draw3D.cpp
	GUI.hpp
	GUI.cpp
	Render.hpp
	Render.cpp)
