#include "VulkanGraphics2D.h"
#include "VulkanGraphics.h"

int main() {
	VulkanGraphics app = VulkanGraphics(2280, 1720, "VulkanGraphics");

	try {
		app.MainLoop();
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
