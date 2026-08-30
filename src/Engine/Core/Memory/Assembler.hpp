#pragma once
#include "Engine/Core/Base.hpp"

#include <asmjit/asmjit.h>

#ifdef PLATFORM_ARCH_X86
	#if defined(PLATFORM_32)
namespace arch = asmjit::x86;
	#elif defined(PLATFORM_64)
namespace arch = asmjit::x86;
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
#define ASM_TRAMPOLINE(hook) arch::ptr(uintptr_t(&hook.Trampoline))

namespace IzEngine
{
	class Assembler
	{
	public:
		static inline asmjit::JitRuntime Runtime;

		template <typename F>
		static uintptr_t NewCode(F function)
		{
			asmjit::CodeHolder code;
			code.init(Runtime.environment(), Runtime.cpuFeatures());
			arch::Assembler a(&code);

			function(code, a);

			void* address = nullptr;
			auto error = Runtime.add(&address, &code);
			if (error)
				throw std::runtime_error("AsmJit compile error.");
			return reinterpret_cast<uintptr_t>(address);
		}
	};
}
