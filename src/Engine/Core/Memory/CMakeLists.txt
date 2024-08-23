add_subdirectory(Implementation)

target_sources(IzEngine PRIVATE
	Assembler.hpp
	Function.hpp
	Hook.hpp
	Memory.hpp
	Signature.hpp
	StackCanary.cpp
	StackCanary.hpp)
