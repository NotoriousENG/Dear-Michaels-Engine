#pragma once

#include <SDL2/SDL.h>
#include <Modules/Render/GL_RenderModule.h>

class SDL_GL_WindowModule
{
public:
	void Init(bool* bQuit, GL_RenderModule* renderModule);
	void Update();
	void Shutdown();

	void GetRendererParams(void*& proc, int& w, int& h);

private:
	bool* bQuit;

	SDL_Window* window;
	SDL_GLContext maincontext;
	SDL_Event event;

	GL_RenderModule* renderModule;

	const int SCREEN_FULLSCREEN = 0;
	int SCREEN_WIDTH = 960;
	int SCREEN_HEIGHT = 540;

	void sdl_die(const char* message);
};

