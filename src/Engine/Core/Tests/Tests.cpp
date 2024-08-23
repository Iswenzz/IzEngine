#include "Base.hpp"

#include "Core/Application/Application.hpp"

void Application::Dispatch(Event& event) { }

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
