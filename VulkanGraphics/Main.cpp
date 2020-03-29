#include "VulkanGraphics.h"

int main() {
	VulkanGraphics app = VulkanGraphics(1600, 1200, "VulkanGarphics");

	try {
		app.VulkanRun();
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

