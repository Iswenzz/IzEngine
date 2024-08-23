#pragma once

namespace IzEngine
{
	/// <summary>
	/// Stack corruption detection.
	/// </summary>
	class StackCanary
	{
	public:
		/// <summary>
		/// Initialize buffer.
		/// </summary>
		StackCanary();

		/// <summary>
		/// Check stack corruption.
		/// </summary>
		~StackCanary() noexcept(false);

	private:
		unsigned char Buffer[50];
	};
}
