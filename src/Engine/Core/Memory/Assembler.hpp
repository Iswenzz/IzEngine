#pragma once
#include "Engine/Base.hpp"
#include "Function.hpp"

#include <asmjit/asmjit.h>
#include <stdexcept>

#ifdef PLATFORM_ARCH_X86
	#if defined(PLATFORM_32)
namespace arch = asmjit::x86;
	#elif defined(PLATFORM_64)
namespace arch = asmjit::x64;
	#endif
#elif PLATFORM_ARCH_ARM
	#if defined(PLATFORM_32)
namespace arch = asmjit::arm;
	#elif defined(PLATFORM_64)
namespace arch = asmjit::aarch64;
	#endif
#endif

#define ASM_FUNCTION(function) void ASM_##function(asmjit::CodeHolder& code, arch::Assembler& a)
#define ASM_LOAD(function) Assembler::NewCode(ASM_##function)

namespace IzEngine
{
	/// <summary>
	/// Just-in-time assembler.
	/// </summary>
	class Assembler
	{
	protected:
		using F = Function<void(asmjit::CodeHolder& code, arch::Assembler& a)>;

	public:
		static inline asmjit::JitRuntime Runtime;

		/// <summary>
		/// Create a new code.
		/// </summary>
		/// <param name="function">The function callback with assembly instructions.</param>
		/// <returns>The compiled function address.</returns>
		static uintptr_t NewCode(F function)
		{
			asmjit::CodeHolder code;
			code.init(Runtime.environment(), Runtime.cpuFeatures());
			arch::Assembler a(&code);

			function(code, a);

			int address = 0;
			auto error = Runtime.add(&address, &code);
			if (error)
				throw std::runtime_error("AsmJit compile error.");
			return address;
		}
	};
}
