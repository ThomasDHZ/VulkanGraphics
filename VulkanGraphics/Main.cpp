#include "VulkanGraphics.h"

int main() {
	VulkanGraphics app(1600,1200, "VulkanGraphics");

	try {
		app.VulkanRun();
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

