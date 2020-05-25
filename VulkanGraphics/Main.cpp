#include "VulkanGraphics2D.h"

int main() {
	VulkanGraphics2D app = VulkanGraphics2D(800, 600, "VulkanGraphics");

	try {
		app.MainLoop();
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
