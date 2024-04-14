add_subdirectory(Implementation)

target_sources(IW3SR.Engine PRIVATE
	Assembler.hpp
	Function.hpp
	Hook.hpp
	Memory.hpp
	Signature.hpp)
