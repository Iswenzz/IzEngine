#pragma once
#include "Frame.hpp"

namespace IzEngine
{
	class API Widget : public Frame
	{
	public:
		Widget();
		virtual ~Widget() = default;

		void Begin() override;
	};
}
