#include "Main.h"

#include <cstdio>
#include <memory>

#include "../Application.h"

#include <SDL2/SDL.h>
#undef main

int main(void)
{
	// Initialize Application
	auto app = std::make_unique<Application>("Michael's App");

	// Start Application loop
	app->Loop();

	return 0;
}
