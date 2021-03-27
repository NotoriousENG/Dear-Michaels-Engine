#include "Application.h"

#include <cstdio>
#include <memory>

Application::Application(const char* app_name)
{
	printf("Created Application (%s)\n", app_name);
	_window = std::make_unique<Window>();
	_window->execute();
}

void Application::Loop()
{
	
}
