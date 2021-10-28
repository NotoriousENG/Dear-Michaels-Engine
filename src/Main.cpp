#include <memory>
#include <SDL2/SDL.h>

#include "Application.h"
#undef main

int main(void)
{
	// Initialize Application
	auto app = std::make_unique<Application>("Michael's App");

	// Start Application loop
	app->Loop();

	return 0;
}
