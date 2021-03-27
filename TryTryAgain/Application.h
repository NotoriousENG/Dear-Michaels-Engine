#pragma once
#include <memory>

#include "Window/Window.h"

class Application
{
public:
	Application(const char* app_name);

	static Application& Instance() { return *_instance; }

	void Loop();

private:
	static Application* _instance;

	// our window
	std::unique_ptr<Window> _window;
};

