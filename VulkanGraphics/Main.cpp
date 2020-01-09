#include "ValkanGraphics.h"

int main() 
{
	ValkanGraphics VGraphics(800, 600, "Vulkan Graphics Testing");
	try
	{
		VGraphics.Run();
	}
	catch (const std::exception & ex)
	{
		std::cerr << ex.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}